#include "fd.h"
#include "string.h"
#include <unistd.h>

void replace_fd(int ori, int target) {
    // prevent origin equals to target
    int tmp = dup(target); 
    close(target);

    // do the replacement
    close(ori);
    dup(tmp);
    close(tmp);
}


void read_and_write(int readfd, int writefd) {
    char buf[READ_BUFFER_SIZE] = { 0 };
    while(read(readfd, buf, READ_BUFFER_SIZE) > 0) {
        write(writefd, buf, READ_BUFFER_SIZE);
        memset(buf, 0, READ_BUFFER_SIZE);
    }
}