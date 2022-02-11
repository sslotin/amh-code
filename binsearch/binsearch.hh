#include <bits/stdc++.h>

#ifndef N
#define N (1<<20)
#endif

void prepare(int *a, int n);
int lower_bound(int x);

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : N);
    int m = (argc > 2 ? atoi(argv[2]) : 1<<20);

    int *a = new int[n];
    int *q = new int[m];

    for (int i = 0; i < n; i++)
        a[i] = rand();
    for (int i = 0; i < m; i++)
        q[i] = rand();

    a[0] = RAND_MAX;
    std::sort(a, a + n);

    prepare(a, n);

    int checksum = 0;
    clock_t start = clock();

    #ifdef LATENCY
    int last = 0;

    for (int i = 0; i < m; i++) {
        last = lower_bound(q[i] ^ last);
        checksum ^= last;
    }
    #else
    for (int i = 0; i < m; i++)
        checksum ^= lower_bound(q[i]);
    #endif

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%.2f ns per query\n", 1e9 * seconds / m);
    printf("%d\n", checksum);
    
    return 0;
}
