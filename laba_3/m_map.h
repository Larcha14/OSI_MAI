#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "function.h"

#define CAPACITY 500
#define SHARED_OBJ1_NAME "/mappa1"
#define SHARED_OBJ2_NAME "/mappa2"

// typedef struct memory_map_struct{
//     int fd;
//     char* buff;
//     int size;
// }memory_map_struct;


typedef struct message{
    int pid;
    int len;
    char buff[CAPACITY];
}message;

typedef struct error_message{
    int pid;
    int status;
}error_message;

// void memory_mapping_creation(memory_map_struct *mm);
// void memory_mapping_cleaning(memory_map_struct *mm);

int write_msg(int pid);
int read_msg(int pid, char *output, int s_len);
int write_error_msg(int pid, int status);
int read_error_msg(int pid);
bool close_sh_file();