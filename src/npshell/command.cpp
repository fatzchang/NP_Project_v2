#include "command.h"
#include "fragment.h"

#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <regex>

// public
command::command(){}

void command::parse(std::string line) {
    std::stringstream ss(line);
    std::string token;
    fragment f;

    std::smatch sm;
    std::regex reg("(\\||!)(\\d+)");

    while (ss >> token) {
        if (token == "|" || token == "!") {
            f.set_err_piping(token == "!");
            add_fragment(f, fragment_position::back);
            // clear and reuse the only fragment
            f.clear();
        } else if (token == ">") {
            // save previous fragment
            add_fragment(f, fragment_position::back);
            f.clear();
            
            std::string filename;
            ss >> filename;
            prepare_output_command(filename);
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
        add_fragment(f, fragment_position::back);
    }
}

// return value is pipe counter, 0 = immediately
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

    return pipe_counter;
}

bool command::hold_turn() {
    return (pipe_counter == 0);
}

void command::decrease_counter() {
    pipe_counter--;
}

int command::get_result() {
    return result_descriptor;
}

int command::get_counter() {
    return pipe_counter;
}

// private
void command::add_fragment(fragment fg, fragment_position position) {
    if (position == fragment_position::back) {
        fragment_list.push_back(fg);
    } else {
        fragment_list.insert(fragment_list.begin(), fg);
    }
}

// insert two commmands, one is at the begining, the other is at the end
void command::prepare_output_command(std::string filename) {
    fragment f_create_file, f_write_file;

    f_create_file.append("create_file");
    f_create_file.append(filename);

    f_write_file.append("write_file");
    f_write_file.append(filename);

    add_fragment(f_create_file, fragment_position::front); // insert to the begining
    add_fragment(f_write_file, fragment_position::back); // insert to the end
}