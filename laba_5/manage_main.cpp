#include <iostream>
#include <string>
#include <zmq.hpp>
#include <unistd.h>
#include <vector>

// #include "awlTree.h"
#include "manage_node.h"
#include "timer.h"

using namespace std;

int process_creation(){
    int pid =fork();
    if(pid==-1){
        perror("Call fork was ended with erorr: ");
        exit(-1);
    }
    return pid;
}

int what_subcmd(string cmd){
    if (cmd=="time"){
        return Subcommands::timer;
    } else if(cmd=="start"){
        return Subcommands::start;
    } else if(cmd=="stop"){
        return Subcommands::stop;
    } 
    return -1;
}



int main(){
    AWL_tree tree;
    Manage_node node;

    cout<<"Welcome in our programm! This is what command i can do:\n";
    cout<<" - create 'id' \n"<<" - exec 'id' 'command(start/stop/time)' \n";
    cout<<" - heartbit 'time' (in ms) \n"<<" - draw\n";
    cout<<"Or enter q or ^D to exit\n"<<"Enter you command: \n";
    
    while (true){
        string cmd;
        cout<<" ->";
        cin>>cmd;
        if(cmd=="create"){
            int id;
            cin>>id;
            if(id<0){
                cout<<"Error: You can only use positive number\n";
            } else if(tree.is_in_tree(id)){
                cout<<"Error: Alredy exists\n";
            } else{
                tree.insert(id);
                // tree.change_availability(id, true);
                int parent_id=tree.parent_id(id);
                if(parent_id==-1){
                    int pid=process_creation();
                    if(pid==0){
                        string id_str=to_string(id);
                        execl("./node", "./node", id_str.c_str(), NULL);
                    } else{
                        cout<<"Ok: "<<pid<<"\n";
                    }
                } else{
                    Message msg;
                    msg.type=Message_type::create;
                    msg.data.push_back(parent_id);
                    msg.data.push_back(id);
                    if(!node.send_msg(msg)){
                        cout<<"Error...\n";
                    }
                    sleep(1);
                }
            }
        } else if(cmd=="exec"){
            int id;
            string subcmd;
            cin>>id;
            cin>>subcmd;
            if(!tree.is_in_tree(id)){
                cout<<"Error: uncorrect id\n";
            } else{
                int subcmd_int=what_subcmd(subcmd);
                if(subcmd_int!=-1){
                    Message msg;
                    msg.type=Message_type::exec;
                    msg.data.push_back(id);
                    msg.data.push_back(subcmd_int);
                    if(!node.send_msg(msg)){
                        cout<<"Error...\n";
                    }
                    sleep(1);
                } else{
                    cout<<"Error: uncorrect subcommands\n";
                }
            }


        } else if(cmd=="heartbit"){
            int period;
            cin>>period;
            if(period<=0){
                cout<<"Error: uncorrect input\n";
            } else{
                Message msg;
                msg.type=Message_type::heartbit;
                msg.data.push_back(-100); // for all users
                msg.data.push_back(period);
                if(!node.send_msg(msg)){
                        cout<<"Error...\n";
                }
                Timer tm;
                tm.start();
                sleep(0.1);
                tree.bypass_reset(tree.get_root());
                while(true){
                    if(tm.times()>=4*period-150){
                        vector<int> unavailable;
                        tree.bypass(tree.get_root(), unavailable);
                        for(int i=0; i< unavailable.size(); ++i){
                            cout << "\033[1;31m";
                            cout<<"Heartbit: node "<<unavailable[i]<<" is unavailable now"<<"\033[0m\n";
                        }
                        if (unavailable.size()==0){
                            cout << "\033[1;32m";
                            cout<<"Heartbit: all nodes are available"<<"\033[0m\n";
                        }
                        // tm.start();
                        tm.stop();
                        break;
                    }
                    node.receive_msg(Message_type::heartbit_answ, tree);
                    
                }


            }
        } else if(cmd=="draw"){
            tree.draw_tree();
            cout<<'\n';
        } else if(cmd=="q" or cin.eof()){
            cout<<"Break;\n";
            break;
        } else{
            cout<<"Uncorrect input. Try again:\n";
        }
    }
}