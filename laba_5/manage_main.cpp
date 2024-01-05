#include <iostream>
#include <string>
#include <zmq.hpp>
#include <unistd.h>


using namespace std;

int main(){
    zmq::context_t context;
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://127.0.0.1:5555");

    zmq::socket_t sub(context, zmq::socket_type::sub);
    sub.bind("tcp://127.0.0.1:5556");
    sub.set(zmq::sockopt::subscribe, "");

    while (true){
        string cmd;
        cout<<" Command: -";
        cin>>cmd;
        if(cmd=="create"){
            int id;
            cin>>id;
            string id_str=to_string(id);
            int pid=fork();
            if(pid==0){
                execl("./node", "./node", id_str.c_str(), NULL);
            } else{
                cout<<"Ok: "<<pid<<'\n';
        

            }
            

        } else if(cmd=="new"){
            int id_parent, id;
            cin>>id_parent>>id;
            string id_str=to_string(id);
            string id_parent_str=to_string(id_parent);

            cout<<"Sending...\n";
            zmq::message_t request1(id_parent_str);
            publisher.send(request1, zmq::send_flags::sndmore);
            zmq::message_t request2(id_str);
            publisher.send(request2, zmq::send_flags::none);

            
            // sleep(1);
            cout<<"Receiving...\n";
            // zmq::message_t reply;
            // zmq::recv_result_t res = sub.recv(reply, zmq::recv_flags::none);
            // if(res==-1){
            //     cout<<"FAIL\n";
            // } else{
            //     string reply_str=reply.to_string();
            //     cout<<"Result: "<<reply_str<<'\n';
            // }
        

        }
        else if(cmd=="q" or cin.eof()){
            break;
        } else{
            cout<<'\n';
        }
    }
}