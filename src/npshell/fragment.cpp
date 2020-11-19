#include "fragment.h"
#include "fd.h"

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <fstream>

// public
fragment::fragment() {
    clear();
}

int fragment::exec() {
    if (token_list.front() == "setenv") {
        const char * env = token_list.at(1).c_str();
        const char * value = token_list.at(2).c_str();
        setenv(env, value, 1);
    } else if (token_list.front() == "printenv") {
        const char * env = token_list.at(1).c_str();
        const char *value = getenv(env);
        std::cout << value << std::endl;
    } else if (token_list.front() == "create_file") {
        std::string filename = token_list.at(1);
        std::ofstream outfile (filename);
        outfile.close();
    } else if (token_list.front() == "write_file") {
        std::string filename = token_list.at(1);
        std::ofstream outfile (filename);
        // TODO: non blocking FIXIT: not good
        char buf[READ_BUFFER_SIZE] = { 0 };
        while (read(input_descriptor, buf, READ_BUFFER_SIZE) > 0) {
            outfile << buf << std::flush;
            memset(buf, 0, READ_BUFFER_SIZE);
        }
        outfile.close();
    } else if (token_list.front() == "exit") {
        exit(0);
    } else {
        exec_bin();
    }
}

void fragment::append(std::string token) {
    token_list.push_back(token);
}

void fragment::set_input(int in_fd) {
    input_descriptor = in_fd;
}

int fragment::get_output() {
    return output_descriptor;
}

void fragment::set_err_piping(bool pipe_err) {
    should_pipe_error = pipe_err;
}

bool fragment::is_empty() {
    return token_list.size() == 0;
}

// TODO: free memory
fragment::~fragment() {
    // if (exec_unit != NULL) {
    //     size_t size_with_null = token_list.size() + 1;

    //     for (size_t i = 0; i < size_with_null; i++) {
    //         free(exec_unit[i]);
    //     }

    //     free(exec_unit);
    // }
}

// private
void fragment::set_output(int out_fd) {
    output_descriptor = out_fd;
}

void fragment::build() {
    size_t size_with_null = token_list.size() + 1;
    
    char **t = (char **)malloc(sizeof(char *) * size_with_null);
    for (size_t i = 0; i < size_with_null; i++) {
        t[i] = (char *)malloc(sizeof(char) * (MAX_ARG_SIZE + 1));
    }

    for (size_t i = 0; i < token_list.size(); i++)
    {
        std::string token = token_list.at(i);
        memset(t[i], 0, MAX_ARG_SIZE + 1);
        token.copy(t[i], MAX_ARG_SIZE);
    }

    t[token_list.size()] = NULL;
    exec_unit = t;
}

void fragment::clear() {
    token_list.clear();
    input_descriptor = STDIN_FILENO; // default: stdin
    output_descriptor = -1;
    should_pipe_error = false;
    exec_unit = NULL;
}

void fragment::exec_bin() {
    build(); // build the command
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "failed to create pipe" << std::endl;
    }

    pid_t pid;
    while ((pid = fork()) < 0) {
        std::cerr << "falied to fork" << std::endl;
        usleep(1000);
    }

    if (pid == 0) {
        close(pipefd[PIPE_READ_END]); // child will never read pipe

        replace_fd(STDIN_FILENO, input_descriptor);
        replace_fd(STDOUT_FILENO, pipefd[PIPE_WRITE_END]);

        if (should_pipe_error) {
            // Do not use pipefd write, because it was closed in previous replace
            close(STDERR_FILENO);
            dup(STDOUT_FILENO);
        }
        
        execvp(exec_unit[0], exec_unit);

        std::cerr << "Unknown command: [" << exec_unit[0] << "]." << std::endl;
        exit(0);
    } else {
        close(pipefd[PIPE_WRITE_END]); // parent will never write pipe
        set_output(pipefd[PIPE_READ_END]);
        waitpid(pid, NULL, 0); // FIXIT: non blocking
    }
}