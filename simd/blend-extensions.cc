#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

const int K = 1e10 / N;

typedef int vec __attribute__ (( vector_size(32) ));

alignas(32) int a[N];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    clock_t start = clock();

    vec *v = (vec*) a;
    vec s = {};

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        for (int i = 0; i < N / 8; i++) {
            s += (v[i] < 50 ? v[i] : 0);
        }
    }

    int s_val = 0;
    for (int i = 0; i < 8; i++)
        s_val += s[i];

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", s_val);

    return 0;
}
