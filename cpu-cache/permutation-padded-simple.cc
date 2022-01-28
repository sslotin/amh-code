#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

const int M = N / 16; // # of memory accesses
const int K = (1<<25) / M;

struct padded_int {
    int val;
    int padding[15];
};

int p[M];
alignas(64) padded_int q[M];
static_assert(sizeof(q) == M * 64);

int main() {
    iota(p, p + M, 0);
    random_shuffle(p, p + M);

    int k = p[M - 1];

    for (int i = 0; i < M; i++)
        k = q[k].val = p[i];

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        #pragma GCC unroll 8
        for (int i = 0; i < M; i++)
            k = q[k].val;
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (M * K));

    return 0;
}