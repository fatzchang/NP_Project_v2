#include "command.h"

#include <iostream>
#include <string>

int npshell() {
    std::string line;
    
    while (1) {
        std::cout << "% " << std::flush;
        getline(std::cin, line);
        command cmd;
        cmd.parse(line);
        cmd.exec(STDIN_FILENO); // TODO: modify input when num piping
    } 
    
    return 0;
}