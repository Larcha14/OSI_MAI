#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "m_map.h"
// #include "timer.h"

using namespace std;

string _MSG_SEND = SH_OBJ_MSG_SEND;
string _MSG_GET = SH_OBJ_MSG_GET;
string _SERVER_PID =SH_OBJ_SERVER_PID;


static int check=0;

void wait_read(int sig){
    check=1;
}

void exit_signal(int sig){
    cout<<"Forced exit\n";
    if(shm_unlink(_SERVER_PID.c_str())==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
    if(shm_unlink(_MSG_SEND.c_str())==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
    if(shm_unlink(_MSG_GET.c_str())==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
    exit(1);
}

struct status{
    int pid;
    bool active;
};

bool is_active_server(map<string, status> &user_dict){
    int status=false;
    for(const auto& [user, user_stat]: user_dict){
        if(user_stat.active){
            status=true;
            break;
        }
    }
    return status;
}

bool is_in_chat(vector<string> &array, string str){
    for(int i=0; i<array.size();++i){
        if(array[i]==str){
            return true;
        }
    }
    return false;
}

void notify_all_in_chat(vector<string> &chat_members, string chat_name, map<string, status> &user_dict){
    for(int i=0; i<chat_members.size();++i){
        int cur_pid=user_dict[chat_members[i]].pid;
        sleep(0.2);
        kill(cur_pid, SIGUSR1);
        cout<<"Notify: "<<cur_pid<<' '<<chat_members[i]<<'\n';
        sleep(0.2);
    }
}

int main(){
    map<string, vector<string>> chat_dict;
    map<string, status> user_dict;
    bool q_server=false;

    int my_pid=getpid();
    // cout<<"Server pid is "<<my_pid<<"\n";

    Memory_map binder(_MSG_SEND, _MSG_GET, _SERVER_PID);
    Msg_pid server_pid;
    // server_pid.Pid=my_pid;
    binder.read_pid(server_pid);
    if(server_pid.Pid!=0){
        cout<<"Server has already run. Exit\n";
        exit(-1);
    } else{
        cout<<"Server pid is "<<my_pid<<"\n";
        server_pid.Pid=my_pid;
    }

    binder.write_pid(server_pid);
    signal(SIGINT, exit_signal);
    while(true){
        Message msg;
        signal(SIGUSR1, wait_read);
        while(check!=1){
            sleep(0.1);
        }
        binder.read_msg(msg);
        cout<<"New_msg: type: "<<msg.type<<" from: <"<<msg.usr_from<<">"<<msg.pid<<"\n";
        check=0;

        Message error_msg;
        error_msg.type=message_type::_error;
        writing(error_msg.to, msg.usr_from, NAMECAPACITY);

        Message answ_msg;
        answ_msg.type=message_type::_server_answer;
        writing(answ_msg.to, msg.usr_from, NAMECAPACITY);
        string answ;
        const char* sender="Server\0";

        string username=writing_to_str(msg.usr_from, NAMECAPACITY);
        cout<<"To: "<<msg.to<<" Data: "<<msg.data<<"\n\n";

        switch(msg.type){
            case message_type::_create :{
                sleep(0.1);
                // sender="Server\0";
                if(user_dict.count(username)>0){ 
                    if(!user_dict[username].active){
                        user_dict[username].pid=msg.pid;
                        user_dict[username].active=true; 
                        // writing(answ_msg.to, msg.usr_from, NAMECAPACITY); 
                        const char *ok_answ="OK: Successful logined!\0";
                        writing(answ_msg.data, ok_answ, 25);
                    } else{
                        const char *bad_answ="Error: User have already logined!\0";
                        writing(error_msg.data, bad_answ, 35);
                        writing(error_msg.usr_from, sender, NAMECAPACITY);
                        binder.write_msg(error_msg);
                        kill(msg.pid, SIGUSR1);
                        cout<<"Send error msg\n";
                        break;
                    }
                } else{
                    user_dict[username].pid=msg.pid;
                    user_dict[username].active=true;
                    const char *ok_answ="OK: Successful created!\0";
                    writing(answ_msg.data, ok_answ, 25);
                }
                writing(answ_msg.usr_from, sender, NAMECAPACITY);

                binder.write_msg(answ_msg);
                kill(user_dict[username].pid, SIGUSR1);
                cout<<"Sending msg:"<<"from sender: "<<sender<<" to "<<msg.pid<<' '<<username<<'\n';
                cout<<"Data: "<<answ_msg.data<<'\n';

                break;
            }
            case message_type::_exited :{
                user_dict[username].active=false; //проверка на количество активных узлов, если 0 - exit
                if(!is_active_server(user_dict)){
                    q_server=true;
                }
                break;
            }
            case message_type::_join_chat :{
                string chat_name=writing_to_str(msg.data, NAMECAPACITY);
                // const char* data_msg;
                string data_msg_str;
                if(chat_dict.count(chat_name)>0){
                    if(is_in_chat(chat_dict[chat_name], username)){
                        data_msg_str="Error: you have already been in this chat\0";
                        writing(answ_msg.data, data_msg_str.c_str(), DATACAPACITY);
                        writing(answ_msg.usr_from, sender, NAMECAPACITY);
                        writing(answ_msg.to, msg.usr_from, NAMECAPACITY);
                        answ_msg.pid=msg.pid;
                        binder.write_msg(answ_msg);
                        kill(answ_msg.pid, SIGUSR1);
                        break;
                    } 
                    //if not
                    sleep(0.1);
                    data_msg_str="New user has joined to chat.\0";
                    writing(answ_msg.data, data_msg_str.c_str(), DATACAPACITY);
                    answ_msg.type=message_type::_msg_to_chat;
                    writing(answ_msg.usr_from, msg.usr_from, NAMECAPACITY);
                    writing(answ_msg.to, msg.data, NAMECAPACITY);
                    binder.write_msg(answ_msg);
                    notify_all_in_chat(chat_dict[chat_name], chat_name, user_dict);
                    // cout<<"=============_has joined to chat=================\n";
                    // cout<<"Data: "<<data_msg_str<<'\n';

                } else{
                    answ_msg.type=message_type::_msg_to_chat;
                    data_msg_str="Chat has been created successfully.";
                    writing(answ_msg.data, data_msg_str.c_str(), DATACAPACITY);
                    writing(answ_msg.usr_from, msg.usr_from, NAMECAPACITY);
                    writing(answ_msg.to, msg.data, NAMECAPACITY);
                    answ_msg.pid=msg.pid;
                    binder.write_msg(answ_msg);
                    kill(answ_msg.pid, SIGUSR1);
                    // cout<<"=============_has created the chat=================\n";
                    // cout<<"Data: "<<data_msg_str<<'\n';
                    

                }
                cout<<"Sending msg:"<<"from sender: "<<answ_msg.usr_from<<" to "<<' '<<msg.data<<'\n';
                cout<<"Data: "<<answ_msg.data<<'\n';
                chat_dict[chat_name].push_back(username);


                break;
            }
            case message_type::_msg_to_usr :{
                string usr_to=writing_to_str(msg.to, NAMECAPACITY);

                if(user_dict.count(usr_to)>0){ 
                    if(user_dict[usr_to].active){
                        writing(answ_msg.usr_from, msg.usr_from, NAMECAPACITY);
                        writing(answ_msg.data, msg.data, DATACAPACITY);
                        writing(answ_msg.to, msg.to, NAMECAPACITY);
                        answ_msg.pid=user_dict[usr_to].pid;
                    } else{
                        const char* data_msg="Error: User is inactive now";
                        writing(answ_msg.to, msg.usr_from, NAMECAPACITY);
                        writing(answ_msg.usr_from, sender, NAMECAPACITY);
                        writing(answ_msg.data, data_msg, 28);
                        answ_msg.pid=msg.pid;
                    }
                } else{
                    const char* data_msg="Error: No such user";
                    writing(answ_msg.to, msg.usr_from, NAMECAPACITY);
                    writing(answ_msg.usr_from, sender, NAMECAPACITY);
                    writing(answ_msg.data, data_msg, 20);
                    answ_msg.pid=msg.pid;

                }
                binder.write_msg(answ_msg);
                kill(answ_msg.pid, SIGUSR1);
                break;
            }
            case message_type::_msg_to_chat :{
                string chat_name=writing_to_str(msg.to, NAMECAPACITY);
                string data_msg_str;
                if(chat_dict.count(chat_name)>0){
                    if(is_in_chat(chat_dict[chat_name], username)){
                        sleep(0.1);
                        writing(answ_msg.data, msg.data, DATACAPACITY);
                        answ_msg.type=message_type::_msg_to_chat;
                        writing(answ_msg.usr_from, msg.usr_from, NAMECAPACITY);
                        writing(answ_msg.to, msg.to, NAMECAPACITY);
                        binder.write_msg(answ_msg);
                        notify_all_in_chat(chat_dict[chat_name], chat_name, user_dict);
                    } else{
                        data_msg_str="Error: You aren't in this chat\0";
                        writing(answ_msg.data, data_msg_str.c_str(), DATACAPACITY);
                        writing(answ_msg.usr_from, sender, NAMECAPACITY);
                        writing(answ_msg.to, msg.usr_from, NAMECAPACITY);
                        answ_msg.pid=msg.pid;
                        binder.write_msg(answ_msg);
                        kill(answ_msg.pid, SIGUSR1);                   
                    }
                } else{
                    data_msg_str="Error: No such chat exist\0";
                    writing(answ_msg.data, data_msg_str.c_str(), DATACAPACITY);
                    writing(answ_msg.usr_from, sender, NAMECAPACITY);
                    writing(answ_msg.to, msg.usr_from, NAMECAPACITY);
                    answ_msg.pid=msg.pid;
                    binder.write_msg(answ_msg);
                    kill(answ_msg.pid, SIGUSR1);                    
                }
                break;
            }
            case message_type::_error:{
                cout<<"<"<<msg.usr_from<<">"<<" was exit with error\n";
                break;
            }
            default:
                break;
            
        }
        if(q_server){
            cout<<"Close server. There are no active users\n";
            break;
        }
        cout<<"Current number of users: "<<user_dict.size()<<"\n\n";
    }

    
    binder.close_sh_file(binder.fn_pid);
    binder.close_sh_file(binder.fn_reading);
    binder.close_sh_file(binder.fn_writing);
}