#ifndef __FRAGMENT_H
#define __FRAGMENT_H
#define MAX_ARG_SIZE (20)

#include <unistd.h>
#include <string>
#include <vector>

class fragment {
public:
    int exec();
    void set_input(int in_fd);
    void set_output(int out_fd);
    void append(std::string token);
    void set_err_piping(bool pipe_err);
    ~fragment();
private:
    std::vector<std::string> token_list;
    int input_descriptor = STDIN_FILENO; // default: stin
    int output_descriptor = STDOUT_FILENO; // default: stdout
    int error_descriptor = STDERR_FILENO; // default: stderr
    bool should_pipe_error = false;
    char **exec_unit;

    void build();
};

#endif