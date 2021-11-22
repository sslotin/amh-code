#include "string.h"

#ifndef N
#define N (1<<20)
#endif

char a[N], b[N];

int main() {
    for (int i = 0; i < 1e9 / N; i++)
        memcpy(a, b, N);

    return 0;
}
