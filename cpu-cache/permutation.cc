#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

int p[N], q[N];

const int K = 2e7 / N;

int main() {
    iota(p, p + N, 0);
    random_shuffle(p, p + N);

    int k = p[N - 1];

    for (int i = 0; i < N; i++) {
        q[k] = p[i];
        k = p[i];
    }

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
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