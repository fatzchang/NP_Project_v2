#include "command.h"
#include "cmanager.h"

#include <iostream>
#include <string>

int npshell() {
    std::string line;
    cmanager cmd_manager;
    setenv("PATH", "bin:.", 1);
    while (1) {
        std::cout << "% " << std::flush;
        getline(std::cin, line);
        if (line.size() > 0) {
            command cmd;
            cmd.parse(line);
            cmd_manager.exec(cmd);
        }
    } 
    
    return 0;
}