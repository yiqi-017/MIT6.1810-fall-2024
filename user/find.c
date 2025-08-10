// user/find.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // 打开当前路径
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // 获取文件/目录状态
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    {
      // 判断文件名是否匹配，打印匹配路径
      char *base = path;
      // 取出路径最后部分，即文件名（类似 basename）
      for(p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
      base = p+1;

      if(strcmp(base, filename) == 0)
        printf("%s\n", path);
    }
    break;

  case T_DIR:
    {
      // 构造目录路径
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';

      // 逐条读取目录项
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        // 跳过 . 和 ..
        if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
          continue;

        // 递归调用
        find(buf, filename);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "usage: find path filename\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
