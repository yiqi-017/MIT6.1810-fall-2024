#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    int n = 10;
    char *error_msg = "please pass an argument\n";
    if(argc > 1){
        n = atoi(argv[1]);
    }else{
        write(2,error_msg,strlen(error_msg));
        exit(0);
    }
    sleep(n);
    exit(0);
}