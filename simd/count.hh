#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

const int K = 1e10 / N;
alignas(32) int a[N];

int count(int x);

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    int checksum = 0;

    clock_t start = clock();

    for (int t = 0; t < K; t++) {
        //__sync_synchronize();
        checksum += count(rand() % 100);
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", checksum);

    return 0;
}
