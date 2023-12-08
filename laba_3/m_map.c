#include "m_map.h"
#include <unistd.h>

int write_msg(int pid){
    int shm_fd=shm_open(SHARED_OBJ1_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // S_IWUSR - пользователь имеет право записи, S_IRUSR - пользователь имеет право чтения, O_RDWR - открывает файл для чтения и записи одновременноc
    if(shm_fd==-1){
        if(shm_unlink(SHARED_OBJ1_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(ftruncate(shm_fd, sizeof(message))==-1){
        if(shm_unlink(SHARED_OBJ1_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    message *msg_ptr=(message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(msg_ptr==MAP_FAILED){
        if(shm_unlink(SHARED_OBJ1_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    // msg_ptr->pid=pid;
    char *s=NULL;
    int s_len=inputing(&s, STDIN_FILENO, 1);
    // msg_ptr->len=s_len;
    if(s_len==-1){
        // char tmp[CAPACITY];
        writing(s, 1, msg_ptr->buff, CAPACITY);
        //write(pipe_1[1], "\n", sizeof("\n"));
        free(s);
        return 1;
    } else if(s_len>=CAPACITY-1){
        write(STDOUT_FILENO, "Sorry, so long message. I'm only lerning and can process message are longer then 255 symbols. Try again\n", 105);
    } else{
        writing(s, s_len, msg_ptr->buff, CAPACITY);
    }
    free(s);
    munmap(msg_ptr, sizeof(message));
    close(shm_fd);
    return 0;
}

int read_msg(int pid, char **input, int s_len){
    int shm_fd=shm_open(SHARED_OBJ1_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // S_IWUSR - пользователь имеет право записи, S_IRUSR - пользователь имеет право чтения, O_RDWR - открывает файл для чтения и записи одновременноc
    if(shm_fd==-1){
        if(shm_unlink(SHARED_OBJ1_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(ftruncate(shm_fd, sizeof(message))==-1){
        if(shm_unlink(SHARED_OBJ1_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    message *msg_ptr=(message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(msg_ptr==MAP_FAILED){
        if(shm_unlink(SHARED_OBJ1_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_fd);
        return -1;
    }
    if(msg_ptr->buff[0]=='\n'){
        return 0;
    }
    if(msg_ptr->buff[0]=='-'){

    }

    // char *output= malloc(CAPACITY*sizeof(char));
    // writing(msg_ptr->buff, CAPACITY, output, s_len);

    char *output=NULL;
    int length=writing_clear(msg_ptr->buff, CAPACITY, &output);

    // write(STDOUT_FILENO, output, sizeof(char)*CAPACITY);
    munmap(msg_ptr, sizeof(message));
    close(shm_fd);
    *input=output;
    return length;

}

int write_error_msg(int pid, char status){
    int shm_err_fd=shm_open(SHARED_OBJ2_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // S_IWUSR - пользователь имеет право записи, S_IRUSR - пользователь имеет право чтения, O_RDWR - открывает файл для чтения и записи одновременноc
    if(shm_err_fd==-1){
        if(shm_unlink(SHARED_OBJ2_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_err_fd);
        return -1;
    }
    if(ftruncate(shm_err_fd, sizeof(error_message))==-1){
        if(shm_unlink(SHARED_OBJ2_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_err_fd);
        return -1;
    }
    error_message *msg_err_ptr=(error_message*)mmap(NULL, sizeof(error_message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_err_fd, 0);
    if(msg_err_ptr==MAP_FAILED){
        if(shm_unlink(SHARED_OBJ2_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_err_fd);
        return -1;
    }
    // msg_err_ptr->pid=pid;
    msg_err_ptr->status=status;
    munmap(msg_err_ptr, sizeof(error_message));
    close(shm_err_fd);
    return 0;


}

int read_error_msg(int pid){
    int shm_err_fd=shm_open(SHARED_OBJ2_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // S_IWUSR - пользователь имеет право записи, S_IRUSR - пользователь имеет право чтения, O_RDWR - открывает файл для чтения и записи одновременноc
    if(shm_err_fd==-1){
        if(shm_unlink(SHARED_OBJ2_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_err_fd);
        return -1;
    }
    if(ftruncate(shm_err_fd, sizeof(error_message))==-1){
        if(shm_unlink(SHARED_OBJ2_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_err_fd);
        return -1;
    }
    error_message *msg_err_ptr=(error_message*)mmap(NULL, sizeof(error_message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_err_fd, 0);
    if(msg_err_ptr==MAP_FAILED){
        if(shm_unlink(SHARED_OBJ2_NAME)==-1){
            perror("munmap trouble: ");
            exit(-1);
        }
        close(shm_err_fd);
        return -1;
    }
    char status=msg_err_ptr->status;
    int res;
    munmap(msg_err_ptr, sizeof(error_message));
    close(shm_err_fd);
    if (status=='0'){
        res=0;
    } else{
        res=1; // error in sentence -> output error
    }
    return res;
}


void close_sh_file(){
    if(shm_unlink(SHARED_OBJ2_NAME)==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
    if(shm_unlink(SHARED_OBJ1_NAME)==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
}
