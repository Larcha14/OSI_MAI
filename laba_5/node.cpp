#include <iostream>
#include <string>
#include <zmq.hpp>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>

#include "msg.h"
#include "timer.h"

using namespace std;


int main(int argc, char *argv[]){
    string my_id_str=argv[1];

    zmq::context_t context;
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.connect("tcp://127.0.0.1:5556");

    zmq::socket_t sub(context, zmq::socket_type::sub);
    sub.connect("tcp://127.0.0.1:5555");
    sub.set(zmq::sockopt::subscribe, my_id_str);    
    sub.set(zmq::sockopt::subscribe, "-100");

    sleep(1);
    int exit_status=prctl(PR_SET_PDEATHSIG, SIGKILL);

    //Timer settings
    Timer tm;
    
    while(true){
        // cout<<"Child: Receiving...\n";
        zmq::message_t reply;
        zmq::recv_result_t res = sub.recv(reply, zmq::recv_flags::none);
        // if(res==-1){
        //     cout<<"FAIL\n";
        // } else{
            string req_id=reply.to_string();

        zmq::message_t cmd;
        res = sub.recv(cmd, zmq::recv_flags::none);
        string cmd_str=cmd.to_string();
        if(cmd_str==to_string(Message_type::create)){
            zmq::message_t new_id;
            res = sub.recv(new_id, zmq::recv_flags::none);
            string new_id_str=new_id.to_string();
            int pid=fork();
            if(pid==0){
                execl("./node", "./node", new_id_str.c_str(), NULL);
            } else{
                // cout<<"Child: Sending...\n";

                string pid_str=to_string(pid);
                // zmq::message_t request(pid_str);
                // publisher.send(request, zmq::send_flags::none);
                cout<<"Ok:"<<my_id_str<<": "<<pid_str<<'\n';

            }
        } else if(cmd_str==to_string(Message_type::exec)){
            zmq::message_t subcmd;
            res = sub.recv(subcmd, zmq::recv_flags::none);
            string subcmd_str=subcmd.to_string();
            cout<<"Ok:"<<my_id_str;
            if(subcmd_str==to_string(Subcommands::timer)){
                cout<<": ";
                cout<<tm.times();

            } else if(subcmd_str==to_string(Subcommands::start)){
                tm.start();

            } else{
                tm.stop();

            }
            cout<<'\n';
        } else if(cmd_str==to_string(Message_type::heartbit)){
            zmq::message_t period_m;
            res = sub.recv(period_m, zmq::recv_flags::none);
            string period_str=period_m.to_string();
            // cout<<"Ok:"<<my_id_str<<"Period: "<<period_str<<'\n';
            int period=stoi(period_str);

            Timer hb_timer;
            hb_timer.start();
            Timer delay;
            delay.start();
            while(true){
                if(delay.times()>=period*4+200){
                    hb_timer.stop();
                    delay.stop();
                    break;
                }
                sleep(0.1);
                if(hb_timer.times()>=period){
                    zmq::message_t request1(my_id_str);
                    publisher.send(request1, zmq::send_flags::none);
                    hb_timer.start();
                }
            }
        }

    }
}