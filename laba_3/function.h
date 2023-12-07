#ifndef function_h
#define function_h
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#define MIN_LEN 10
#define MAX_LEN 255
#define SIGTERM 15

#define RED_COLOR    "\x1b[31m" 
#define GREEN_COLOR  "\x1b[32m" 
#define RESET_COLOR    "\x1b[0m" 

extern const char*  message_list[6];

void pipe_creation(int *fd);
int process_creation();        
int str_size(const char *string);
bool clean_name(char **output_name, char* input_name);
void check_res(int fd_in, int fd_out);
int inputing(char **s_output, int fd, int endl_status);
bool writing(char *from, int from_size ,char* to, int to_size);


#endif