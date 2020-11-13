#ifndef __COMMAND_H
#define __COMMAND_H

#include "fragment.h"
#include <vector>

class command {
public:
    command();
    int exec();
    void parse(std::string line);

private:
    std::vector<fragment> fragment_list;
    void add_fragment(fragment fg);
    int pipe_counter;
};

#endif