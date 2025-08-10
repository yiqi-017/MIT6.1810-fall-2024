#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    int fds[2];
    char send_byte = 'a';
    pipe(fds);
    int pid = fork();
    if(pid==0){//在子进程里面
        char revd;
        read(fds[0],&revd,1);
        printf("%d: received ping\n",getpid());
        write(fds[1],&revd,1);
    }else if(pid>0){
        char revdbyp;
        write(fds[1],&send_byte,1);
        wait(0);
        read(fds[0],&revdbyp,1);
        printf("%d: received pong\n",getpid());
    }else{
        exit(1);
    }
    exit(0);
    return 0;
}