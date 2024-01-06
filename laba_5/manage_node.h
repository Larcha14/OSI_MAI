#pragma once
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <sys/wait.h>
#include <zmq.hpp>
#include "msg.h"
#include "awlTree.h"

using namespace std;


class Manage_node{
    public:
        zmq::context_t context;
        zmq::socket_t publisher;
        zmq::socket_t sub;
        
        bool send_msg(Message msg); 
        void receive_msg(Message_type msg_type, AWL_tree &tree);


        Manage_node();
        ~Manage_node();

};