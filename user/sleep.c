#include <kernel/types.h>
#include "user/user.h"

int main(int argc, char *argv[]) {
    
    // atoi - changes string to integer
    int x = atoi(argv[1]);
    
    if (argc != 2) {
        write(2, "Error message", strlen("Error message"));
    }
    
    sleep(x);
    exit();
}