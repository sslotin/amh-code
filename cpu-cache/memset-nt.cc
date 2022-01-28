#include <stdio.h>
#include <time.h>
#include <cstring>
#include <x86intrin.h>

#ifndef N
#define N (1<<20)
#endif

const int K = 1e10 / N;

int a[N];

int main() {
    clock_t start = clock();

    int s = 0;

    const __m256i zeros = _mm256_set1_epi32(0);

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        // memset(a, 0, sizeof a);
        #pragma GCC unroll(4)
        for (int i = 0; i + 7 < N; i += 8)
            _mm256_stream_si256((__m256i*) &a[i], zeros);
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", s);

    return 0;
}
