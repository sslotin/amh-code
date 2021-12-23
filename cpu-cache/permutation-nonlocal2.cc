#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 16
#endif

static_assert(D >= 16 && D % 16 == 0);

const int M = N / D;
const int K = (1<<24) / N;
const int T = D / 16;

int p[M], q[N], t[T];

int main() {
    iota(p, p + M, 0);
    random_shuffle(p, p + M);

    iota(t, t + T, 0);

    int k = p[M - 1];

    for (int l = 0; l < 16; l++) {
        random_shuffle(t, t + T);
        for (int i = 0; i < T; i++) {
            for (int j = 0; j < M; j++) {
                k = q[k] = D * p[j] + 16 * t[i] + l;
            }
        }
    }

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