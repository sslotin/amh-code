#include <stdio.h>
#include <time.h>

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 1
#endif

const int K = 1e9 / N;
const int M = D * N;

int a[M];

int main() {
    clock_t start = clock();

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        #pragma GCC unroll(4)
        for (int i = 0; i < M; i += D)
            a[i] += 1;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.4f GFLOPS\n", 1e-9 * N * K / duration);

    return 0;
}
