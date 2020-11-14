#ifndef __FRAGMENT_H
#define __FRAGMENT_H
#define MAX_ARG_SIZE (20)

#include <unistd.h>
#include <string>
#include <vector>

class fragment {
public:
    fragment();
    int exec();
    void set_input(int in_fd);
    int get_output();
    void append(std::string token);
    void set_err_piping(bool pipe_err);
    bool is_empty();
    void clear();
    ~fragment();
private:
    std::vector<std::string> token_list;
    int input_descriptor;
    int output_descriptor;
    bool should_pipe_error;
    char **exec_unit;

    void set_output(int out_fd);
    void build();
    void exec_bin();
};

#endif