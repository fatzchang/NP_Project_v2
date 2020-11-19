#include "npshell.h"
#include "reaper.h"
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // register a signal handler to kill zombie
    signal(SIGCHLD, reaper);

    npshell();
    
    return 0;
}