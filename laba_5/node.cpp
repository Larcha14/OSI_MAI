#include <iostream>
#include <string>
#include <zmq.hpp>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>

using namespace std;

int main(int argc, char *argv[]){
    string my_id_str=argv[1];

    zmq::context_t context;
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.connect("tcp://127.0.0.1:5556");

    zmq::socket_t sub(context, zmq::socket_type::sub);
    sub.connect("tcp://127.0.0.1:5555");
    sub.set(zmq::sockopt::subscribe, my_id_str);    

    sleep(1);
    int exit_status=prctl(PR_SET_PDEATHSIG, SIGKILL);
    
    while(true){
        // cout<<"Child: Receiving...\n";
        zmq::message_t reply;
        zmq::recv_result_t res = sub.recv(reply, zmq::recv_flags::none);
        // if(res==-1){
        //     cout<<"FAIL\n";
        // } else{
            string req_id=reply.to_string();

        zmq::message_t reply2;
        res = sub.recv(reply2, zmq::recv_flags::none);
        string new_id=reply2.to_string();

        int pid=fork();
        if(pid==0){
            execl("./node", "./node", new_id.c_str(), NULL);
        } else{
            // cout<<"Child: Sending...\n";

            string pid_str=to_string(pid);
            // zmq::message_t request(pid_str);
            // publisher.send(request, zmq::send_flags::none);
            cout<<"Ok: "<<pid_str<<'\n';

        }
        // }
    }
}