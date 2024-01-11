#pragma once
#include <iostream>
#include <string>
#include <signal.h>

#define DATACAPACITY 300
#define NAMECAPACITY 30

#define SH_OBJ_MSG_SEND "/writer" //
#define SH_OBJ_MSG_GET "/reader" // название функций с точки зрения сервера
#define SH_OBJ_SERVER_PID "/myserver_pid"

using namespace std;

enum message_type{
    _create,
    _join_chat,
    _msg_to_usr,
    _msg_to_chat,
    _exited,
    _error,
    _server_answer
};

struct Message{
    message_type type;
    int pid;
    char to[NAMECAPACITY];
    char usr_from[NAMECAPACITY];
    char data[DATACAPACITY];
};


struct Msg_pid{
    int Pid;
};

