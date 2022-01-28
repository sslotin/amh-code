#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <x86intrin.h>

#ifndef N
#define N (1<<20)
#endif

const int K = 1e10 / N;

typedef int vec __attribute__ (( vector_size(32) ));
const int B = 2;

int a[N];

int main() {
    clock_t start = clock();

    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    int s = 0;

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        
        vec b[B] = {0};
        vec* v = (vec*) a;

        #pragma GCC unroll(8)
        for (int i = 0; i < N / 8; i += B) {
            for (int j = 0; j < B; j++) {
                vec t = (vec) _mm256_stream_load_si256((__m256i*) &v[i + j]);
                b[j] += t;
            }
        }

        for (int i = 1; i < B; i++)
            b[0] += b[i];
        
        for (int i = 0; i < 8; i++)
            s += b[0][i];
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", s);

    return 0;
}
