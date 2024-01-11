#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include <stdlib.h>

#include "msg.h"

 
class Memory_map{
    public:
        int my_pid;
        int server_pid;
        Memory_map(string fn_wr, string fn_re, string fn_p);

        bool write_msg(Message &msg);
        bool read_msg(Message &msg);

        bool write_pid(Msg_pid &msg);
        bool read_pid(Msg_pid &msg);

        void close_sh_file(string fn);

        ~Memory_map();
    // private:
        string fn_writing;
        string fn_reading;
        string fn_pid;
};

 void writing(char *to,const char *from, int size);
 string writing_to_str(char *from, int size);