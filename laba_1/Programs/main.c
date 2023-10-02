#include <stdio.h>
#include "function.h"

int main(){
    char str_input[MAX_LEN];
    write(STDOUT_FILENO, "Enter filename with file extension: ", 37);
    read(STDIN_FILENO, str_input, MAX_LEN);
    char *Filename=NULL;
    if(clean_name(&Filename, str_input)==false){
        perror("Trying to create 0-value string: ");
        exit(-1);
    }
    int f_input=open(Filename, O_WRONLY | O_CREAT, 0777);
    // FILE* f_input =fopen(Filename, "w");
    if(f_input==-1){
        fprintf(stderr, "Can't open the file:  %s", Filename);
        exit(-1);
    }        
    int pipe_1[2], pipe_2[2];
    pipe_creation(pipe_1);
    pipe_creation(pipe_2);
    int pid=process_creation();
    if(pid==0){
        // printf("Its child\n");
        close(pipe_1[1]); //unsed fd_pipe_1 for writing
        close(pipe_2[0]);//unsed fd_pipe_2 for reading
        if(dup2(pipe_1[0], STDIN_FILENO)==-1){
            
            perror("Call dup2 was ended with erorr: ");
            exit(-1);
        }
        if(dup2(f_input, STDOUT_FILENO)==-1){
            perror("Call dup2 was ended with erorr: ");
            exit(-1);
        }
        if(dup2(pipe_2[1], STDERR_FILENO)==-1){
            perror("Call dup2 was ended with erorr: ");
            exit(-1);
        }
          
        if(execl("./child", "./child", NULL)==-1){
            perror("Call execl was ended with erorr: ");
            exit(-1);
        }
        close(pipe_1[0]);
        close(pipe_2[1]);
        close(f_input);

    }else{ //it's parant
        // printf("Its parant");
        close(f_input);
        close(pipe_1[0]); //unsed fd_pipe_1 for reading
        close(pipe_2[1]); //unsed fd_pipe_2 for writing

        write(pipe_1[1], Filename, str_size(Filename));

        wait(NULL);
        check_res(pipe_2[0], STDOUT_FILENO);
        // write(STDOUT_FILENO, "\n", sizeof("\n"));
        write(STDOUT_FILENO, GREEN_COLOR, sizeof(GREEN_COLOR));
        write(STDOUT_FILENO, "\n", sizeof("\n"));
        write(STDOUT_FILENO, message_list[5], str_size(message_list[5]));
        write(STDOUT_FILENO, "\n", sizeof("\n"));
        write(STDOUT_FILENO, RESET_COLOR, sizeof(RESET_COLOR));
        close(pipe_1[1]);
        close(pipe_2[0]);
    }



}