#include <stdio.h>
#include "function.h"
#include "m_map.h"

bool check_first_size(char a){
    if(a>='A' && a<='Z'){
        return true;
    }
    return false;
}

int main(int argc,const char* argv[]){
    int ppid1=atoi(argv[1]);
    // write(STDOUT_FILENO, argv[1], sizeof(char)*8);
    // while(1){
        //char output[CAPACITY]; // хуйня идея заполниться стек
        
        char *output= malloc(CAPACITY*sizeof(char));
        int read_status=read_msg(0, output, CAPACITY);
        if(read_status==-1){
            free(output);
            perror("Something is going wrong: ");
            exit(-1);
        } else if(read_status==1){
            free(output);
            // break;
        }

        if(check_first_size(output[0])==true){
            // write(STDOUT_FILENO, output, sizeof(char)*CAPACITY);
            write_error_msg(0, 0);
            //write(STDERR_FILENO, "0", sizeof("0"));
        } else{
            write_error_msg(0, 1);
            // write(STDERR_FILENO, "1", sizeof("1")); //uncorrect input
        }
        free(output);
        signal(SIGUSR1, read_msg);
        // while(1);
    // }
    return 0;
}

/*
идея как исправить:
    будем при каждом чтении строки, при записи её и передачи в дочку записывать спец символ
    как обычно - "-". В дочке считываем его, если он не равен, ему то все, продолжаем.
*/