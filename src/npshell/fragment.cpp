#include "fragment.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

int fragment::exec() {
    // replace
    close(STDIN_FILENO);
    dup(input_descriptor);
    close(STDOUT_FILENO);
    dup(output_descriptor);
    close(STDERR_FILENO);
    dup(error_descriptor);


    if (token_list.at(0) == "setenv") {
        const char * env = token_list.at(1).c_str();
        const char * value = token_list.at(2).c_str();
        setenv(env, value, 1);
    } else if (token_list.at(0) == "printenv") {
        const char * env = token_list.at(1).c_str();
        const char *value = getenv(env);
        std::cout << value << std::endl;
    } else {
        build(); // build the command
        execvp(exec_unit[0], exec_unit);
    }
}

void fragment::append(std::string token) {
    token_list.push_back(token);
}

void fragment::set_input(int in_fd) {
    input_descriptor = in_fd;
}

void fragment::set_output(int out_fd) {
    output_descriptor = out_fd;
}

void fragment::set_err_piping(bool pipe_err) {
    should_pipe_error = pipe_err;
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

fragment::~fragment() {
    size_t size_with_null = token_list.size() + 1;

    for (size_t i = 0; i < size_with_null; i++) {
        free(exec_unit[i]);
    }

    free(exec_unit);
}