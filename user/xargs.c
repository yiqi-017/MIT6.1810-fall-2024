// user/xargs.c
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs command [args...]\n");
    exit(1);
  }

  char buf[512];
  char *cmdargv[MAXARG];
  int i;

  // 先把命令和命令参数复制到cmdargv
  for(i = 1; i < argc && i < MAXARG - 1; i++){
    cmdargv[i - 1] = argv[i];
  }

  // 读取输入行缓冲区索引
  int n = 0;

  while(1){
    char c;
    int r = read(0, &c, 1);
    if(r < 1){
      // 输入结束，退出循环
      break;
    }

    if(c == '\n' || n == sizeof(buf) - 1){
      // 到行尾或者缓冲区满，构成一条完整的参数行
      buf[n] = 0; // 终止字符串

      // 构造执行命令的argv数组
      // 先复制已有命令参数
      for(i = 0; i < argc - 1; i++){
        cmdargv[i] = argv[i + 1];
      }
      cmdargv[argc - 1] = buf;  // 追加当前行作为最后一个参数
      cmdargv[argc] = 0;        // 结尾NULL

      int pid = fork();
      if(pid < 0){
        fprintf(2, "fork failed\n");
        exit(1);
      }
      else if(pid == 0){
        exec(cmdargv[0], cmdargv);
        fprintf(2, "exec %s failed\n", cmdargv[0]);
        exit(1);
      }
      else{
        wait(0);
      }
      n = 0; // 重置缓冲区索引
    }
    else{
      buf[n++] = c;
    }
  }

  exit(0);
}
