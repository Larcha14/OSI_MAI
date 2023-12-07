#include <stdio.h>
#include "function.h"
#include "m_map.h"

void check_res(){
    
}


int main(){
    write(STDOUT_FILENO, "Enter filename with file extension: ", 37);

    char *Filename=NULL;
    if(inputing(&Filename ,STDIN_FILENO, 0)<=0){
        perror("Trying to create 0-value string: ");
        exit(-1);
    }
    int f_input=open(Filename, O_WRONLY | O_CREAT, 0777);
    // FILE* f_input =fopen(Filename, "w");
    if(f_input==-1){
        fprintf(stderr, "Can't open the file:  %s", Filename);
        exit(-1);
    }        
    int parent_pid=getpid();
    char s_ppid[sizeof(char)*8];
    sprintf(s_ppid, "%d", parent_pid); // int в строку чаров 
    // int tmp=atoi(s_ppid);
    // printf("\n%s", s_ppid);
    int pid=process_creation();
    if(pid==0){
        // printf("Its child\n");

        if(dup2(f_input, STDOUT_FILENO)==-1){
            perror("Call dup2 was ended with erorr: ");
            exit(-1);
        }

        if(execl("./child", "./child", s_ppid, NULL)==-1){
            perror("Call execl was ended with erorr: ");
            exit(-1);
        }


    }else{ //it's parant
        // printf("Its parant");
        close(f_input);
        write(STDOUT_FILENO, "Enter something you want: \n", 28);

        while(true){ // \n\n - ending
            int write_state=write_msg(pid); // -1 - erorr, 0 -OK, 1 - ending
            if(write_state==1){
                break;
            } else if(write_state==-1){
                perror("Something is going wrong: ");
                exit(-1);
            }
            // sleep(1);
            kill(pid, SIGUSR1);
            signal(SIGUSR2, read_error_msg(pid));
            int read_err_state=read_error_msg(pid);
            if(read_err_state ==0){

            } else if( read_err_state==1){
                write(STDOUT_FILENO, RED_COLOR, sizeof(RED_COLOR));
                write(STDOUT_FILENO, message_list[0], sizeof(char)*str_size(message_list[0]));
                write(STDOUT_FILENO, RESET_COLOR, sizeof(RESET_COLOR));
                write(STDOUT_FILENO, "\n", sizeof("\n"));
            } else{
                perror("Something is going wrong: ");
                exit(-1);           
            }
        }
        write(STDOUT_FILENO, GREEN_COLOR, sizeof(GREEN_COLOR));
        write(STDOUT_FILENO, "\n", sizeof("\n"));
        write(STDOUT_FILENO, message_list[5], sizeof(char)*str_size(message_list[5]));
        write(STDOUT_FILENO, "\n", sizeof("\n"));
        write(STDOUT_FILENO, RESET_COLOR, sizeof(RESET_COLOR));
        
        kill(pid, SIGTERM);
    }
    close_sh_file();


    
}