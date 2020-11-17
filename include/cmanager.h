#ifndef __CMDLIST_H
#define __CMDLIST_H

#include "command.h"
#include <vector>
class cmanager {
public:
    void exec(command cmd);

private:
    std::vector<command> cmd_list;
    bool need_to_pipe = false; // exist a command's pipe counter = 0

    void next_round();
    int gather_result();
};


#endif