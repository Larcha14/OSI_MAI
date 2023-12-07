#include "m_map.h"
#include <unistd.h>

// extern struct memory_map_struct;

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
    msg_ptr->pid=pid;
    char *s=NULL;
    int s_len=inputing(&s, STDIN_FILENO, 1);
    msg_ptr->len=s_len;
    if(s_len==-1){
        // char tmp[CAPACITY];
        writing(s, s_len, msg_ptr->buff, CAPACITY);
        //write(pipe_1[1], "\n", sizeof("\n"));
        free(s);
        return 1;
    } else if(s_len>=CAPACITY){
        write(STDOUT_FILENO, "Sorry, so long message. I'm only lerning and can process message are longer then 255 symbols. Try again\n", 105);
    } else{
        writing(s, s_len, msg_ptr->buff, CAPACITY);
    }
    free(s);
    munmap(msg_ptr, sizeof(message));
    close(shm_fd);
    return 0;
}

int read_msg(int pid, char *output, int s_len){
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
        return 1;
    }
    if(msg_ptr->buff[0]=='-'){

    }
    writing(msg_ptr->buff, CAPACITY, output, s_len);
    munmap(msg_ptr, sizeof(message));
    close(shm_fd);
    return 0;

}

int write_error_msg(int pid, int status){
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
    msg_err_ptr->pid=pid;
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
    int status=msg_err_ptr->status;
    munmap(msg_err_ptr, sizeof(error_message));
    close(shm_err_fd);
    return status;
}


bool close_sh_file(){
    if(shm_unlink(SHARED_OBJ2_NAME)==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
    if(shm_unlink(SHARED_OBJ1_NAME)==-1){
        perror("munmap trouble: ");
        exit(-1);
    }
}

// void memory_mapping_creation(memory_map_struct *mm){
//     char filename[]=map_name;
//     mm->fd=shm_open(filename, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR); 
//     // S_IWUSR - пользователь имеет право записи, S_IRUSR - пользователь имеет право чтения, O_RDWR - открывает файл для чтения и записи одновременноc
//     if(mm->fd==-1){
//         perror("Memory map creating error");
//         exit(-1);
//     }
//     mm->size=CAPACITY;
//     mm->buff=(char*) mmap(NULL, mm->size, PROT_READ | PROT_WRITE, MAP_SHARED, mm->fd, 0);

//     if (mm->buff == MAP_FAILED){
//         perror("Mmap error");
//         exit(-1);
//     }
//     // printf("%d \n", sb.st_size);




// }

// void memory_mapping_cleaning(memory_map_struct *mm){
//     if(shm_unlink(map_name)==-1){
//         perror("hm_error error");
//         exit(-1);
//     }
//     if(munmap(mm->buff, mm->size)==-1){
//         perror("Munmap error");
//         exit(-1);
//     }

// }

// void memory_mapping_open(memory_map_struct ){}

// int main(){
//     memory_map_struct mm;
//     memory_mapping_creation(&mm);
//     memory_mapping_cleaning(&mm);
// }
