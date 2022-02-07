#include <bits/stdc++.h>

#ifndef n
#define n (1<<16)
#endif

alignas(64) int a[n];

int argmin();

int main() {
    #ifdef DEC
    for (int i = 0; i < n; i++)
        a[i] = n - i;
    #else
    for (int i = 0; i < n; i++)
        a[i] = rand();
    #endif

    volatile int k = argmin();
    
    int m = (0 <= k && k < n ? a[k] : -1);
    printf("%d %d\n", k, m);

    clock_t start = clock();

    int cnt = (1ll << 31) / n;

    for (int i = 0; i < cnt; i++)
        k = argmin();

    float avg = float(clock() - start) / CLOCKS_PER_SEC / cnt;

    printf("%.8f x %d\n", avg, cnt);
    printf("%.2f GFLOPS\n", 1e-9 * n / avg);

    return 0;
}
