#ifndef __COMMAND_H
#define __COMMAND_H

#include "fragment.h"
#include <vector>

class command {
public:
    command();
    int exec(int input_fd);
    void parse(std::string line);

private:
    std::vector<fragment> fragment_list;
    int pipe_counter = 0;
    
    void add_fragment(fragment fg);
};

#endif