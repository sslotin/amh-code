#include <bits/stdc++.h>

int argmin(int *a, int n);

#ifndef N
#define N (1<<16)
#endif

const int K = 1e9 / N;

alignas(64) int a[N];

int main() {
    #ifdef DEC
    for (int i = 0; i < N; i++)
        a[i] = N - i;
    #else
    for (int i = 0; i < N; i++)
        a[i] = rand();
    #endif

    volatile int k = argmin(a, N);
    
    int m = (0 <= k && k < N ? a[k] : -1);
    printf("%d %d\n", k, m);

    clock_t start = clock();

    for (int i = 0; i < K; i++)
        k = argmin(a, N);

    float avg = float(clock() - start) / CLOCKS_PER_SEC / K;

    printf("%.2f GFLOPS\n", 1e-9 * N / avg);

    return 0;
}
