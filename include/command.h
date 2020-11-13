#include <vector>

class fragment {
public:
private:
    int input_descriptor = STDIN_FILENO;
    int output_descriptor = STDOUT_FILENO;

}

class command {
public:
    command() {}

private:
    std::vector<fragment> fragment_list;
    add_fragment();
}