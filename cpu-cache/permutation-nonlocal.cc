#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 16
#endif

const int M = N / D;
const int K = (1<<24) / N;

int p[M], q[N];

int main() {
    iota(p, p + M, 0);
    random_shuffle(p, p + M);

    int k = p[M - 1];

    for (int i = 0; i < D; i++)
        for (int j = 0; j < M; j++)
            k = q[k] = D * p[j] + i;

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        #pragma GCC unroll 8
        for (int i = 0; i < N; i++) {
            //__builtin_prefetch(&a[i + 10 * D]);
            k = q[k];
            //cerr << k << endl;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}