#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 1
#endif

const int M = N / D;
const int K = (1<<27) / N;

int p[M], q[D][M];

int main() {
    int k[D];
    
    for (int d = 0; d < D; d++) {
        iota(p, p + M, 0);
        random_shuffle(p, p + M);
        k[d] = p[M - 1];
        for (int i = 0; i < M; i++)
            k[d] = q[d][k[d]] = p[i];
    }

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        for (int i = 0; i < M; i++)
            for (int d = 0; d < D; d++)
                k[d] = q[d][k[d]];
        for (int d = 0; d < D; d++)
            s ^= k[d];
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}