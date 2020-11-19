#include <sys/wait.h>

void reaper(int signal) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {}
}