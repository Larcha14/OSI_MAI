#include "m_map.h"

void writing(char *to,const char *from, int size){ // *to and *from have equal capacity
    for(int i=0; i< size;++i){
        if(from[i]==EOF or from[i]=='\0' or from[i]=='\n'){
            to[i]='\0';
            break;
        }
        to[i]=from[i];
    }
    to[size-1]='\0';

}

 string writing_to_str(char *from, int size){
    string tmp;
    for(int i=0; i<size;++i){
        if(from[i]==EOF or from[i]=='\0' or from[i]=='\n'){
            tmp+='\0';
            break;
        }
        tmp+=from[i];
    }
    return tmp;
 }

Memory_map::Memory_map(string fn_wr, string fn_re, string fn_p){
    fn_writing=fn_wr;
    fn_reading=fn_re;
    fn_pid=fn_p;

}

Memory_map::~Memory_map(){

}
 
bool Memory_map::read_pid(Msg_pid &msg){
    int shm_fd=shm_open( fn_pid.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    if(shm_fd==-1){
        if(shm_unlink( fn_pid.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(ftruncate(shm_fd, sizeof(Msg_pid))==-1){
        if(shm_unlink( fn_pid.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    Msg_pid *msg_ptr=(Msg_pid*)mmap(NULL, sizeof(Msg_pid), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(msg_ptr==MAP_FAILED){
        if(shm_unlink( fn_pid.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    msg.Pid=msg_ptr->Pid;
    munmap(msg_ptr, sizeof(Msg_pid));
    close(shm_fd);
    return true;
}

bool Memory_map::write_pid(Msg_pid &msg){
    int shm_fd=shm_open( fn_pid.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    if(shm_fd==-1){
        if(shm_unlink( fn_pid.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(ftruncate(shm_fd, sizeof(Msg_pid))==-1){
        if(shm_unlink( fn_pid.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    Msg_pid *msg_ptr=(Msg_pid*)mmap(NULL, sizeof(Msg_pid), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(msg_ptr==MAP_FAILED){
        if(shm_unlink( fn_pid.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    msg_ptr->Pid=msg.Pid;
    munmap(msg_ptr, sizeof(Msg_pid));
    close(shm_fd);
    return true;
}

bool Memory_map::write_msg(Message &msg){
    int shm_fd=shm_open( fn_writing.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    if(shm_fd==-1){
        if(shm_unlink( fn_writing.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(ftruncate(shm_fd, sizeof(Message))==-1){
        if(shm_unlink( fn_writing.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    Message *msg_ptr=(Message*)mmap(NULL, sizeof(Message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(msg_ptr==MAP_FAILED){
        if(shm_unlink( fn_writing.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    msg_ptr->type=msg.type;
    msg_ptr->pid=msg.pid;
    writing( msg_ptr->to, msg.to, NAMECAPACITY);
    writing(msg_ptr->usr_from, msg.usr_from, NAMECAPACITY);
    writing(msg_ptr->data, msg.data, DATACAPACITY);
    munmap(msg_ptr, sizeof(Message));
    close(shm_fd);
    return true;
}

 
bool Memory_map ::read_msg(Message &msg){
    int shm_fd=shm_open( fn_reading.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    if(shm_fd==-1){
        if(shm_unlink( fn_reading.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(ftruncate(shm_fd, sizeof(Message))==-1){
        if(shm_unlink( fn_reading.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    Message *msg_ptr=(Message*)mmap(NULL, sizeof(Message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(msg_ptr==MAP_FAILED){
        if(shm_unlink( fn_reading.c_str())==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    msg.type=msg_ptr->type;
    msg.pid=msg_ptr->pid;
    writing(msg.to, msg_ptr->to, NAMECAPACITY);
    writing(msg.usr_from, msg_ptr->usr_from, NAMECAPACITY);
    writing(msg.data, msg_ptr->data, DATACAPACITY);
    munmap(msg_ptr, sizeof(Message));
    close(shm_fd);
    return true;
}

void Memory_map::close_sh_file(string fn){
    if(shm_unlink(fn.c_str())==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
}