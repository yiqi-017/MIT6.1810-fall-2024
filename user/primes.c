#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define Max_Value 280
void primes(int p_read) __attribute__((noreturn)); // 声明该函数不会返回

void primes(int p_read){
    int prime,n,fds[2];
    if(read(p_read,&prime,sizeof(prime))!=sizeof(prime)){
        close(p_read);
        exit(0);
    }
    printf("prime %d\n",prime);
    if(pipe(fds)<0){
        printf("pipe failed");
        exit(1);
    }
    int pid = fork();
    if(pid == 0){
        close(fds[1]);
        close(p_read);
        primes(fds[0]);
    }else{
        close(fds[0]);
        while(read(p_read,&n,sizeof(n))==sizeof(n)){
            if(n%prime){
                write(fds[1],&n,sizeof(n));
            }
        }
        close(fds[1]);
        close(p_read);
        wait(0);
        exit(0);
    }
}

int main(){
    int fds[2];
    if(pipe(fds)<0){
        printf("pipe failed");
        exit(1);
    }
    int pid = fork();
    if(pid<0){
        printf("make child failed");
        exit(1);
    }
    if(pid==0){//子进程只读不写
        close(fds[1]);
        primes(fds[0]);
    }else{
        close(fds[0]);
        for(int i = 2;i<=Max_Value;i++){
            write(fds[1],&i,sizeof(i));
        }
        close(fds[1]);
        wait(0);
        exit(0);
    }
    return 0;
}
