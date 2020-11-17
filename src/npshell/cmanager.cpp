#include "cmanager.h"
#include "command.h"
#include "fd.h"
#include <unistd.h>
#include <vector>
#include <iostream>
#include <string.h>

void cmanager::exec(command cmd) {
    int input_fd = gather_result(); // gather result from num pipe list
    if (cmd.exec(input_fd) == 0) {
        // immediately print the result if it's not a num pipe
        int result_fd = cmd.get_result();
        char buf[READ_BUFFER_SIZE];
        while(read(result_fd, buf, READ_BUFFER_SIZE) > 0) {
            std::cout << buf << std::flush;
        }
    } else {
        cmd_list.push_back(cmd);
    }

    next_round();
}


void cmanager::next_round() {
    need_to_pipe = false;
    std::vector<command>::iterator it;
    for (it = cmd_list.begin(); it != cmd_list.end(); it++) {
        if (it->get_counter() == 0) {
            cmd_list.erase(it); // clear finished commands
            it--;
            continue;
        }

        it->decrease_counter();
        if (it->get_counter() == 0) {
            need_to_pipe = true; 
        }
    }
}

int cmanager::gather_result() {
    int fd = STDIN_FILENO;

    if (need_to_pipe) {
        int pipefd[2];
        pipe(pipefd);

        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "fork failed" << std::endl;
        } else if (pid == 0) { // child
            std::vector<command>::iterator it;
            char buf[READ_BUFFER_SIZE];
            for (it = cmd_list.begin(); it != cmd_list.end(); it++) {
                if (it->hold_turn()) {
                    while(read(it->get_result(), buf, READ_BUFFER_SIZE) > 0) {
                        write(pipefd[PIPE_WRITE_END], buf, READ_BUFFER_SIZE);
                        memset(buf, 0, READ_BUFFER_SIZE);
                    }
                }
            }

            close(pipefd[PIPE_READ_END]);
            close(pipefd[PIPE_WRITE_END]);

            exit(0);
        } else {
            close(pipefd[PIPE_WRITE_END]);
            fd = pipefd[PIPE_READ_END];
            // TODO: non block waiting
        }
    }
    
    return fd;
}