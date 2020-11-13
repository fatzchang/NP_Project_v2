#include "command.h"

#include <iostream>
#include <string>

int npshell() {
    std::string line;
    
    while (1) {
        getline(std::cin, line);
        command cmd;
        cmd.parse(line);
    } 
    
    return 0;
}