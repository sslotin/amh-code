#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

const int K = 1e10 / N;

typedef __m256i reg;

alignas(32) int a[N];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    clock_t start = clock();

    int s = 0;

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        for (int i = 0; i < N; i++)
            s += (a[i] < 50 ? a[i] : 0);
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", s);

    return 0;
}
