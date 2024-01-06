#include "manage_node.h"


Manage_node::Manage_node(): publisher(context, zmq::socket_type::pub), sub(context, zmq::socket_type::sub){
    publisher.bind("tcp://127.0.0.1:5555");
    sub.bind("tcp://127.0.0.1:5556");
    //фильтр для сообщений, подписываемся на все сообщения 
    sub.set(zmq::sockopt::subscribe, "");
}

Manage_node::~Manage_node(){
    sub.disconnect("tcp://127.0.0.1:5556");
    publisher.disconnect("tcp://127.0.0.1:5555");
}

void Manage_node::receive_msg(Message_type msg_type, AWL_tree &tree){
    cout<<"Receiving...\n";
    switch(msg_type){
        case Message_type::heartbit_answ :{
            zmq::message_t reply;
            zmq::recv_result_t res = sub.recv(reply, zmq::recv_flags::none);
            string id_str=reply.to_string();
            int id=stoi(id_str);
            tree.change_availability(id, true);
            break;

        }
    }
}

bool Manage_node::send_msg(Message msg){
    cout<<"Sending...\n";
    string type_str=to_string(msg.type);
    switch(msg.type){
        case Message_type::create :{

            string parent_id_str=to_string(msg.data[0]);
            string new_id_str=to_string(msg.data[1]);

            zmq::message_t request1(parent_id_str);
            publisher.send(request1, zmq::send_flags::sndmore);
            zmq::message_t request2(type_str);
            publisher.send(request2, zmq::send_flags::sndmore);
            zmq::message_t request3(new_id_str);
            publisher.send(request3, zmq::send_flags::none);

            return true;
        }
        case Message_type::exec :{
            string id_str=to_string(msg.data[0]);
            string subcmd_str=to_string(msg.data[1]);

            zmq::message_t request1(id_str);
            publisher.send(request1, zmq::send_flags::sndmore);
            zmq::message_t request2(type_str);
            publisher.send(request2, zmq::send_flags::sndmore);
            zmq::message_t request3(subcmd_str);
            publisher.send(request3, zmq::send_flags::none);

            return true;

        }
        case Message_type::heartbit :{
            string id_str=to_string(msg.data[0]);
            string period_str=to_string(msg.data[1]);

            zmq::message_t request1(id_str);
            publisher.send(request1, zmq::send_flags::sndmore);
            zmq::message_t request2(type_str);
            publisher.send(request2, zmq::send_flags::sndmore);
            zmq::message_t request3(period_str);
            publisher.send(request3, zmq::send_flags::none);

            return true;


        }

        default:
            return false;
    }
    return false;

}
