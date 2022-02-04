#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

#ifndef Q
#define Q (1<<26)
#endif

int a[N], q[Q];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand();

    for (int i = 0; i < Q; i++)
        q[i] = rand() % N;

    int checksum = 0;

    clock_t start = clock();

    for (int i = 0; i < Q; i++) {
        checksum += a[q[i]];
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * Q / duration);

    printf("%d\n", checksum);

    return 0;
}
