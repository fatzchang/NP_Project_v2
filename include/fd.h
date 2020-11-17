#ifndef __FD_H
#define __FD_H

#define PIPE_READ_END (0)
#define PIPE_WRITE_END (1)
#define READ_BUFFER_SIZE (1024)

void replace_fd(int ori, int target);
void read_and_write(int readfd, int writefd);

#endif