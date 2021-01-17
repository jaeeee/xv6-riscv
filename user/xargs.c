#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {

     char *max_argv_size[MAXARG];
     int argv_length = 0;
     char buffer[128] = {0};

      while(gets(buffer, sizeof(buffer))){
          int buffer_length = strlen(buffer);
        //   char *c = buffer;

        char *x = buffer;
          if (buffer_length <= 0) {
              break;
          }
          buffer[buffer_length - 1] = 0;
          argv_length = argc - 1;
        //   while (*c) {
        //       while(*c && (*c == ' ')) {
        //           *c++ = 0;
        //       }
        //       if (*c) {
        //           max_argv_size[argv_length++] = c;
        //       }
        //       while (*c && (*c != ' ')) {
        //           c++;
        //       } 
        //     //   while 
        //   }

          while(*x){
      while(*x && (*x == ' ')) *x++ = 0;
      if(*x) max_argv_size[argv_length++] = x;
      while(*x && (*x != ' ')) x++;
    }

          if (argv_length <= 0) {
              printf("too few args (xargs)\n");
              exit();
          }
          if (argv_length >= MAXARG) {
              printf("too many args (xargs) \n");
              exit();
          }

          max_argv_size[argv_length] = 0;
          if (fork()) {
              wait();
          } else {
              exec(max_argv_size[0], max_argv_size);
              exit();
          }
      }

    exit();
}