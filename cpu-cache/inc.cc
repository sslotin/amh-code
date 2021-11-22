#include <stdio.h>
#include <time.h>

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 1
#endif

const int K = 5e9 / N;

int a[N];

int main() {
    clock_t start = clock();

    for (int t = 0; t < K; t++) {
        /* For some reason, if a is defined and used as a global variable,
           GCC fuses the outer loop in groups of two (applying a[i] += 2 instead of a[i] += 1).
           This is fine for relative speed comparisons, but since we want to measure memory bandwidth,
           we need to put a memory fence there. */
        __sync_synchronize();
        #pragma GCC unroll(4)
        for (int i = 0; i < N; i += D)
            a[i] += 1;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    return 0;
}
