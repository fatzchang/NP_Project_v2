#include "cmanager.h"
#include "command.h"
#include <vector>
#include <iostream>

void cmanager::exec(command cmd) {
    std::vector<command>::iterator it;
    for (it = cmd_list.begin(); it != cmd_list.end(); it++) {
        if (it->hold_turn()) {
            // read the output from it->get_result
            // it will auto decrease
        }
    }

    // TODO: change parameter to the real input
    if (cmd.exec(0) == 0) {
        // immediately print the result if it's not a num pipe
        int result_fd = cmd.get_result();
        char buf[READ_BUFFER_SIZE];
        while(read(result_fd, buf, READ_BUFFER_SIZE) > 0) {
            std::cout << buf << std::flush;
        }
    } else {
        cmd_list.push_back(cmd);
    }
}