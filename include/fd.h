#ifndef __FD_H
#define __FD_H

#define PIPE_READ_END (0)
#define PIPE_WRITE_END (1)

void replace_fd(int ori, int target);

#endif