#include "read.hh"

int read(int n) {
    const int SIZE = (1<<14); // 16K
    char buf[SIZE];

    int s = 0, x = 0;

    while (true) {
        int len = fread(buf, 1, SIZE, stdin);
        
        if (len != SIZE) {
            int k = 0;
            while (k < len) {
                char c = buf[k++];
                if (c != '\n') // c != '\n' may be one cycle shorter
                    x = 10 * x + (c - '0');
                else {
                    s ^= x;
                    x = 0;
                }
            }
            break;
        } else {
            int k = len / 2;
            while (buf[k - 1] != '\n')
                k--;

            int l = 0, r = k;
            int y = 0;

            while (l < k) {
                char a = buf[l++], b = buf[r++];

                if (a != '\n')
                    x = 10 * x + (a - '0');
                else {
                    s ^= x;
                    x = 0;
                }

                if (b != '\n')
                    y = 10 * y + (b - '0');
                else {
                    s ^= y;
                    y = 0;
                }
            }

            while (r < SIZE) {
                char b = buf[r++];
                if (b != '\n')
                    y = 10 * y + (b - '0');
                else {
                    s ^= y;
                    y = 0;
                }
            }

            x = y;
        }
    }

    return s; // never reached
}
