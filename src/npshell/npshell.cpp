#include <iostream>
#include <string>
#include <regex>

int npshell() {
    std::string token;
    // token < fragment < command
    while (std::cin >> token) {
        std::cout << token << std::endl;
        if (token == "\n") {
            // execute
            // command -> execute
        } else {

        }
    }
    
    return 0;
}