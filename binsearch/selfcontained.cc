#include <ctime>
#include <cstdio>
#include <algorithm>

// play with the array size (n) and compiler versions
const int n = 1e6, m = 2e6;

int a[n], q[m], results[m];
alignas(64) int t[n + 1];

void build(int k = 1) {
    static int i = 0;
    if (k <= n) {
        build(2 * k);
        t[k] = a[i++];
        build(2 * k + 1);
    }
}

int baseline(int x) {
    return *std::lower_bound(a, a + n, x);
}

int branchless(int x) {
    int *base = a, len = n;
    while (len > 1) {
        int half = len / 2;
        base = (base[half] < x ? &base[half] : base);
        len -= half;
    }
    return *(base + (*base < x));
}

int eytzinger(int x) {
    int k = 1;
    while (k <= n) {
        __builtin_prefetch(t + k * 16);
        k = 2 * k + (t[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return t[k];
}

float timeit(int (*lower_bound)(int)) {
    clock_t start = clock();

    int checksum = 0;

    for (int i = 0; i < m; i++)
        checksum += lower_bound(q[i]);
    
    float duration = float(clock() - start) / CLOCKS_PER_SEC;

    printf("  checksum: %d\n", checksum);
    printf("  latency: %.2fns\n", 1e9 * duration / m);

    return duration;
}

int main() {
    for (int i = 0; i < n; i++)
        a[i] = rand(); // <- careful on 16-bit platforms
    for (int i = 0; i < m; i++)
        q[i] = rand();
    
    a[0] = RAND_MAX; // to avoid dealing with end-of-array iterators 
 
    std::sort(a, a + n);
    build();

    printf("std::lower_bound:\n");
    float x = timeit(baseline);
    
    printf("branchless:\n");
    printf("  speedup: %.2fx\n", x / timeit(branchless));
 
    printf("eytzinger:\n");
    printf("  speedup: %.2fx\n", x / timeit(eytzinger));
 
    return 0;
}
