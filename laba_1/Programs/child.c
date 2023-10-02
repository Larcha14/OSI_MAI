#include <stdio.h>
#include "function.h"

bool check_first_size(char a){
    if(a>='A' && a<='Z'){
        return true;
    }
    return false;
}

int main(){
    while(1){
        char input_name[MAX_LEN];
        read(STDIN_FILENO, input_name, sizeof(input_name));

        char* output_name=NULL;
        if(check_first_size(input_name[0])==true){

            clean_name(&output_name, input_name);
            write(STDOUT_FILENO, output_name, str_size(output_name));
            break;
        } else{
            write(STDERR_FILENO, "\0", sizeof("\0"));
            break;
        }
    }
    return 0;
}