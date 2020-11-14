#include "command.h"
#include "fragment.h"

#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>

// public
command::command(){}

void command::parse(std::string line) {
    std::stringstream ss(line);
    std::string token;
    fragment f;

    while (ss >> token) {
        if (token == "|" || token == "!") {
            f.set_err_piping(token == "!");
            add_fragment(f);
            // clear and reuse the only fragment
            f.clear();
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

    /**
     * TODO: if counter == 0, cout the last pipe content
     * e.g.
     * char buf[100];
     * while(read((it - 1)->get_output(), buf, 100) > 0)
     *     std::cout << buf << std::endl;
     * 
     **/
}

// private
void command::add_fragment(fragment fg) {
    fragment_list.push_back(fg);
}