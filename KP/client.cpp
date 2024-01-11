#include <iostream>
#include "m_map.h"

using namespace std;

string _MSG_SEND = SH_OBJ_MSG_GET;
string _MSG_GET = SH_OBJ_MSG_SEND;
string _SERVER_PID = SH_OBJ_SERVER_PID;

pthread_mutex_t mutex;

static int wf=0;
static bool error_exit=false;

void wait_sig(int sig){
    wf=1;
}


bool check_name(string name){
    if(name.size()>NAMECAPACITY){
        return false;
    }
    for(int i=0;i<name.size();++i){
        if (name[i]=='\n' or name[i]==' '){
            return false;
        }
    }
    return true;
}

void* receiving(void* args){
    Memory_map *binder=(Memory_map*) args;
    // sleep(1);
    while(true){
        Message msg;
        // cout<<"Start waiting sig...\n";
        signal(SIGUSR1, wait_sig);
        // signal(SIGUSR1, wait_msg);
        while(wf!=1){
            sleep(0.1);
        }
        // cout<<"Got sig\n";
        // sleep(0.2);
        // pthread_mutex_lock(&mutex);
        binder->read_msg(msg);
        if(msg.type==message_type::_error){
            cout<<"Error:\n";
            cout<<msg.usr_from<<":"<<msg.data;
            error_exit=true;
            break;
        } else if(msg.type==message_type::_msg_to_chat){
            cout<<"New message:\n"<<"Chat:"<<msg.to<<":"<<msg.usr_from<<' '<<msg.data;
        } else{
            cout<<"New message:\n";
            cout<<msg.usr_from<<":"<<msg.data;
        }
        // fflush(NULL);
        // pthread_mutex_unlock(&mutex);
        wf=0;

    }

}

int main(int argc, char* argv[]){
    if(argc!=2){
        cout<<"Uncorrect input. Input your name!\n";
        exit(-1);
    }
    string username = argv[1];
    if(!check_name(username)){
        cout<<"Uncorrect username\n";
        exit(-1);
    }
    Memory_map binder(_MSG_SEND,_MSG_GET, _SERVER_PID );
    Msg_pid server_pid;
    binder.read_pid(server_pid);
    if(server_pid.Pid==0){
        cout<<"Server is unavailable now...\n";
        exit(-1);
    }
    int my_pid = getpid();
    binder.my_pid=my_pid;
    binder.server_pid=server_pid.Pid;
    
    cout<<"Conecting...\n";
    Message init;
    writing(init.usr_from, username.c_str(), username.size()+1);
    init.type=message_type::_create;
    init.pid=my_pid;

    // signal(SIGUSR1, wait_sig); 
    pthread_mutex_init(&mutex, NULL);
    pthread_t receiver;
    if(pthread_create(&receiver, NULL, receiving, &binder)!=0){
        perror("Create thread error ");
    }
    
    binder.write_msg(init);
    kill(server_pid.Pid, SIGUSR1);

    // usleep(2000);
    sleep(2);

    pthread_mutex_lock(&mutex);
    cout<<"Server_pid: "<<server_pid.Pid<<'\n';
    cout<<"Welcome in our chat, "<<username<<"!\n";
    cout<<"Here you can communicate with other users directly or using chats\n\n";
    cout<<"--For reading messages write:\n"<<"To:'other_username' 'you message...'\n";
    cout<<"To:chat:'chat_name' 'you message...'\n"<<"-- Or if you want to join or creata a chat write:\n";
    cout<<"Join:'chat_name'\n";
    cout<<"--Write 'q' to close terminal\n";
    pthread_mutex_unlock(&mutex);
    while(true){
        Message msg;
        msg.pid=my_pid;
        writing(msg.usr_from, username.c_str(), username.size()+1);

        if(error_exit){
            msg.type=message_type::_error;
            pthread_cancel(receiver);
            pthread_detach(receiver);
            binder.write_msg(msg);
            kill(server_pid.Pid, SIGUSR1);
            cout<<"Break;";
            break;
        }
        string request;
        cout<<" > ";
        getline(cin, request);
        bool currect=true;
        if(request=="q" or cin.eof()){
            msg.type=message_type::_exited;
            pthread_cancel(receiver);
            pthread_detach(receiver);
            binder.write_msg(msg);
            kill(server_pid.Pid, SIGUSR1);
            cout<<"Break;\n";
            break;
        } else if(request.substr(0, 8)=="To:chat:"){
            msg.type=message_type::_msg_to_chat;
            int pos=request.find(" ");
            string chat_rec=request.substr(8, pos-8);
            if(pos!= string::npos){
                string data_str=request.substr(pos+1, request.size()-(pos+1));
                if(data_str.size()==0){
                    cout<<"Uncorrect input\n";
                    currect=false;
                } else{
                    writing(msg.to, chat_rec.c_str(), chat_rec.size()+1);
                    writing(msg.data, data_str.c_str(), data_str.size()+1);
                }
            } else{
                cout<<"Uncorrect input\n";
                currect=false;
            }
        } else if(request.substr(0, 3)=="To:"){
            msg.type=message_type::_msg_to_usr;
            int pos=request.find(" ");
            string usr_rec=request.substr(3, pos-3);
            if(pos!= string::npos){
                string data_str=request.substr(pos+1, request.size()-(pos+1));
                if(data_str.size()==0){
                    cout<<"Uncorrect input\n";
                    currect=false;
                } else{
                    writing(msg.to, usr_rec.c_str(), usr_rec.size()+1);
                    writing(msg.data, data_str.c_str(), data_str.size()+1);
                }
            } else{
                cout<<"Uncorrect input\n";
                currect=false;
            }
        } else if(request.substr(0, 5)=="Join:"){
            msg.type=message_type::_join_chat;
            string chat_name=request.substr(5, request.size()-5);
            if(chat_name.size()==0){
                cout<<"Uncorrect input\n";
                currect=false;
            } else{
                writing(msg.data, chat_name.c_str(), chat_name.size()+1);
            }
        }else if(request==""){
            cout<<'\n';
            currect=false;
        } else{
            cout<<"Uncorrect input\n";
            currect=false;
        }
        if(currect){
            binder.write_msg(msg);
            kill(server_pid.Pid, SIGUSR1);
            sleep(1);
        }
    }

}