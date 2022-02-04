#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

#ifndef P
#define P 50
#endif

const int K = 2e9 / N;
alignas(32) int a[N], b[N];

int filter();

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    clock_t start = clock();

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        filter();
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    // test
    
    int s = 0;

    int k = filter();

    for (int i = 0; i < k; i++)
        s ^= b[i] * i;

    printf("%d %d\n", k, s);

    return 0;
}
