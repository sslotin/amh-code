#include <stdio.h>
#include <time.h>
#include <cstring>

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 1
#endif

const int K = 1e10 / N;

int a[N];

int main() {
    clock_t start = clock();

    int s = 0;

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        // memset(a, 0, sizeof a);
        #pragma GCC unroll(4)
        for (int i = 0; i < N; i += D)
            a[i] = 0;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", s);

    return 0;
}
