#include "read.hh"

void prepare() {}

int read(int n) {
    const int SIZE = (1<<14); // 16K
    char buf[SIZE];

    int s = 0;

    while (s < n) {
        int L = fread(buf, 1, SIZE, stdin);
        L = (L < SIZE ? L : SIZE);
        int k = 0;
        while (k < L) 
            if (buf[k++] == '\n')
                s++;
    }

    return s;
}
