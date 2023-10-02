#include <stdio.h>
#include "function.h"

void pipe_creation(int *fd){
    if(pipe(fd)==-1){
        perror("Call pipe was ended with error: ");
        exit(-1);
    }

}

int process_creation(){
    int pid =fork();
    if(pid==-1){
        perror("Call fork was ended with erorr: ");
        exit(-1);
    }
    return pid;
}

int str_size(const char *string){
    int len=0;
    for(int i=0; i<MAX_LEN; ++i){ // Fix reading '\n' bag and input string lenth counting
        if(string[i]=='\n' || string[i]==EOF || string[i]=='\0'){
            break;
        }
        len++;
    }
    return len;
}

bool clean_name(char **output_name, char* input_name){
    int len=str_size(input_name);
    if(len==0){
        return false;
    }
    char tmp[len+1];
    for(int i=0; i<len;++i){
        tmp[i]=input_name[i];
    }
    tmp[len]='\0';
    free(*output_name);
    *output_name=tmp;
    return true;


}

void check_res(int fd_in, int fd_out){
    char str_input[MAX_LEN];
    read(fd_in, str_input, MAX_LEN);
    int len=str_size(str_input);
    // printf("%d", len);
    if(len!=0){
        char message[]="Correct filename.\n";
        write(fd_out, GREEN_COLOR, sizeof(GREEN_COLOR));
        write(fd_out, message, str_size(message));
        write(fd_out, RESET_COLOR, sizeof(RESET_COLOR));
    }else{
        write(fd_out, RED_COLOR, sizeof(RED_COLOR));
        write(fd_out, message_list[0], str_size(message_list[0]));
        write(fd_out, RESET_COLOR, sizeof(RESET_COLOR));
    }

}

const char*  message_list[]={
    //Errors:
    "Erorr!_Uncorrect input. Filename must begin with a capital letter!\n",
    "Call pipe was ended with error: ",
    "Call fork was ended with erorr: ",
    "Trying to create 0-value string: ",
    //Normal status
    "Enter filename with file extension: ",
    "Program was ended succsessfully!\n\n",

};
