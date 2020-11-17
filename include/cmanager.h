#ifndef __CMDLIST_H
#define __CMDLIST_H

#include "command.h"
#include <vector>
class cmanager {
public:
    void exec(command cmd);

private:
    std::vector<command> cmd_list;
};


#endif