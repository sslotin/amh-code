#include "read.hh"
#include <sys/mman.h>

int read(int n) {
    // n is not used
    int fsize = lseek(0, 0, SEEK_END);
    auto input = (char*) mmap(0, fsize, PROT_READ, MAP_PRIVATE, 0, 0);

    int s = 0, x = 0;

    for (int i = 0; i < fsize; i++) {
        char c = input[i];
        if (c >= '0' && c <= '9') // c != '\n' may be one cycle shorter
            x = 10 * x + (c - '0');
        else {
            s ^= x;
            x = 0;
        }
    }

    return s;
}
