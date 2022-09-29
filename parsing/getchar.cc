#include "read.hh"

int read(int n) {
    int s = 0, x = 0;

    while (n) {
        int c = getchar();
        if (c >= '0' && c <= '9')
            x = 10 * x + (c - '0');
        else {
            s ^= x;
            x = 0;
            n--;
        }
    }

    return s;
}
