#include "read.hh"

int read(int n) {
    const int SIZE = (1<<14); // 16K
    char buf[SIZE];

    int s = 0, x = 0;

    while (true) {
        fread(buf, 1, SIZE, stdin);
        int k = 0;

        while (k < SIZE) {
            char c = buf[k++];
            if (c >= '0' && c <= '9') // c != '\n' may be one cycle shorter
                x = 10 * x + (c - '0');
            else {
                s ^= x;
                x = 0;
                n--;
                if (n == 0)
                    return s;
            }
        }
    }

    return s; // never reached
}
