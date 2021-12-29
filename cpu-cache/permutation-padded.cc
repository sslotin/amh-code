#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 1
#endif

const int M = N / D / 16; // # of memory accesses
const int K = (1<<25) / M;

struct padded_int {
    int val;
    int padding[15];
};

int p[M];
alignas(64) padded_int q[M][D];
static_assert(sizeof(q) == M * D * 64);

int main() {
    
    iota(p, p + M, 0);
    random_shuffle(p, p + M);

    int k = p[M - 1];

    for (int i = 0; i < M; i++)
        k = q[k][0].val = p[i];

    for (int i = 0; i < M; i++) {
        for (int j = 1; j < D; j++) {
            q[i][j].val = rand();
            q[i][0].val ^= q[i][j].val;
        }
    }

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        #pragma GCC unroll 8
        for (int i = 0; i < M; i++) {
            //__builtin_prefetch(&a[i + 10 * D]);
            int x = 0;
            for (int j = 0; j < D; j++)
                x ^= q[k][j].val;
            //cerr << k << " " << q[k][0] << " " << q[k][1] << endl;
            k = x;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (M * K));

    return 0;
}