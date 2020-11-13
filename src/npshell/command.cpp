#include "command.h"
#include "fragment.h"

#include <string>
#include <sstream>
#include <unistd.h>

command::command(){}

void command::parse(std::string line) {
    std::stringstream ss(line);
    std::string token;
    fragment f;

    while (ss >> token) {
        if (token == "|" || token == "!") {
            f.set_err_piping(token == "!");
            fragment_list.push_back(f);
        } else {
            f.append(token);
        }
    }
}

int command::exec() {
    // TODO: piping
}