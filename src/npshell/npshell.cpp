#include "command.h"
#include "cmanager.h"

#include <iostream>
#include <string>

int npshell() {
    std::string line;
    cmanager cmd_manager;
    
    while (1) {
        std::cout << "% " << std::flush;
        getline(std::cin, line);
        command cmd;
        cmd.parse(line);
        cmd_manager.exec(cmd);
        // cmd.exec(STDIN_FILENO); // TODO: modify input when num piping
    } 
    
    return 0;
}