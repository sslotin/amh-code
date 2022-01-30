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
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    clock_t start = clock();
    int s = 0;

    for (int t = 0; t < K; t++) {
        __sync_synchronize();

        /*
        vec* v = (vec*) a;
        volatile __m256i x;

        #pragma GCC unroll (8)
        for (int i = 0; i < N / 8; i++) {
            x = _mm256_load_si256((__m256i*) &v[i]);
        }
        */


        /*
        vec* v = (vec*) a;
        //__m256i __attribute__((used)) x;

        __m256 x;

        #pragma GCC unroll (8)
        for (int i = 0; i < N / 8; i++) {
            __m256 *ptr = (__m256*) &v[i];
            asm volatile ("vmovdqa (%1), %0" : "=r" (x) : "r" (ptr));
        }
        */        
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
