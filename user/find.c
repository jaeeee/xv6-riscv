#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int checkFile(char* directory, char* file) {
    char *ptr;
    ptr = directory+strlen(directory);
    while (ptr >= directory && *ptr != '/') {
        ptr--;
    }
   ptr++;
    if (strcmp(ptr, file) == 0) {
        return 1;
    }
    return 0;
}

void find(char* path, char* name){
  char buf[64], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_FILE) {

      if (checkFile(path, name)) {
          printf("%s\n", path);
      }

  } else if (st.type == T_DIR) {
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name);
    }
  }
  close(fd);
}


int
main(int argc, char *argv[])
{
  if(argc < 3){
    exit();
  }
  find(argv[1], argv[2]);
  exit();
}