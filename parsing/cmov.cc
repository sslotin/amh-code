#include "read.hh"

int read(int n) {
    const int SIZE = (1<<14); // 16K
    char buf[SIZE];

    int s = 0, x = 0;

    while (true) {
        int buf_len = fread(buf, 1, SIZE, stdin);
        int k = 0;

        if (buf_len != SIZE)
            break;

        while (k < SIZE) {
            char c = buf[k++];
            
            bool cond = (c == '\n');

            s ^= (cond ? x : 0);

            x = 10 * x + (c - '0');
            x = (cond ? 0 : x);

            //n -= cond;

            //if (n == 0)
            //    return s;
        }
    }

    return s; // never reached
}
