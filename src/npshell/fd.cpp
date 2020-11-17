#include <unistd.h>

void replace_fd(int ori, int target) {
    // prevent origin equals to target
    int tmp = dup(target); 
    close(target);

    // do the replacement
    close(ori);
    dup(tmp);
    close(tmp);
}