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

bool writing(char *from, int from_size ,char* to, int to_size){
    for(int i=0; i<from_size; ++i){
        to[i]=from[i];
    }
    return true;
}

int inputing(char **s_output, int fd, int endl_status){
    int new_l=MIN_LEN;
    char *line=(char*)malloc(sizeof(char)*new_l);
    int i=0;
    char ch;
    read(fd, &ch, sizeof(ch));
    if(ch=='\n'){
        line[i]='\n';
        *s_output=line;
        return -1;
    }
    while(ch!=EOF && ch!='\0' && ch!='\n' ){
        if(i>=new_l){
            new_l=new_l*2;
            line=(char *)realloc(line, sizeof(char)*new_l);
        }
        line[i]=ch;
        i++;
        read(fd, &ch, sizeof(ch));

    }
    if(endl_status!=0){
        if(i>=new_l){
            new_l=new_l*2;
            line=(char *)realloc(line, sizeof(char)*new_l);
        }
        line[i]='\n';
        i++;
    }
    *s_output=line;
    return i;
    
}


void check_res(int fd_in, int fd_out){
    char status;
    read(fd_in, &status, sizeof(char));
    if(status=='1'){
        write(fd_out, RED_COLOR, sizeof(RED_COLOR));
        write(fd_out, message_list[0], str_size(message_list[0]));
        write(fd_out, RESET_COLOR, sizeof(RESET_COLOR));
        write(fd_out, "\n", sizeof("\n"));
    }

}

const char*  message_list[]={
    //Errors:
    "Error!_Uncorrect input.\n",
    "Call pipe was ended with error: ",
    "Call fork was ended with error: ",
    "Trying to create 0-value string: ",
    //Normal status
    "Enter filename with file extension: ",
    "Program was ended successfully!\n\n",

};
