#include "command.h"
#include "fragment.h"

#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>

#include <iostream>
#include <regex>

// public
command::command(){}

void command::parse(std::string line) {
    std::stringstream ss(line);
    std::string token;
    fragment f;

    std::smatch sm;
    std::regex reg("(\\||!)(\\d+)"); // TODO: will have to implement '>'

    while (ss >> token) {
        if (token == "|" || token == "!") {
            f.set_err_piping(token == "!");
            add_fragment(f);
            // clear and reuse the only fragment
            f.clear();
        } else if (regex_match(token, sm, reg)) {
            // deal with num pipe |xxx
            std::string symbo = sm[1].str();
            pipe_counter = stoi(sm[2].str());
            f.set_err_piping(symbo == "!");
        } else {
            f.append(token);
        }
    }

    // skip empty line
    if (!f.is_empty()) {
        add_fragment(f);
    }
}

int command::exec(int input_fd) {
    std::vector<fragment>::iterator it;
    for (it = fragment_list.begin(); it != fragment_list.end(); it++) {
        if (it != fragment_list.begin()) {
            input_fd = (it - 1)->get_output();
        }

        it->set_input(input_fd);
        it->exec();
    }

    // save the last output of fragment as the result output
    result_descriptor = (it - 1)->get_output();

    // immediately print the result if it's not a num pipe
    if (pipe_counter == 0) {
        char buf[READ_BUFFER_SIZE];
        while(read(result_descriptor, buf, READ_BUFFER_SIZE) > 0)
            std::cout << buf << std::flush;
    }
}

bool command::hold_turn() {
    bool my_turn = (pipe_counter == 0);

    if (!my_turn) {
        pipe_counter--;
    }

    return my_turn;
}

// private
void command::add_fragment(fragment fg) {
    fragment_list.push_back(fg);
}