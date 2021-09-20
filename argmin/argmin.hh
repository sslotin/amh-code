#include <bits/stdc++.h>

const int n = (1<<16);
alignas(64) int a[n];

int argmin();

int main() {
    for (int i = 0; i < n; i++)
        a[i] = rand();
    
    volatile int k = argmin();
    
    int m = (0 <= k && k < n ? a[k] : -1);
    printf("%d %d\n", k, m);

    clock_t start = clock();

    int cnt = (1 << 30) / n;

    for (int i = 0; i < cnt; i++)
        k = argmin();

    float avg = float(clock() - start) / CLOCKS_PER_SEC / cnt;

    printf("%.8f x %d\n", avg, cnt);
    printf("%.2f GFLOPS\n", 1e-9 * n / avg);

    return 0;
}
