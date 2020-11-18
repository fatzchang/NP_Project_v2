#ifndef __COMMAND_H
#define __COMMAND_H


#include "fragment.h"
#include <vector>


class command {
enum class fragment_position {back, front};

public:
    command();
    int exec(int input_fd);
    void parse(std::string line);
    int get_result();
    int get_counter();
    bool hold_turn();
    void decrease_counter();

private:
    std::vector<fragment> fragment_list;
    int pipe_counter = 0;
    int result_descriptor = -1;
    
    void add_fragment(fragment fg, fragment_position position);
    void prepare_output_command(std::string filename);
};

#endif