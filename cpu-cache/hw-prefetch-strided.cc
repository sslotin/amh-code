#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

int q[N];

const int K = 2e7 / N;

const int B = 256;

int main() {
    for (int i = 0; i < N; i++)
        q[i] = (i + B < N ? i + B : (i + 17) % B);
    
    clock_t start = clock();

    volatile int s = 0;
    int k = 0;

    for (int t = 0; t < K; t++) {
        for (int i = 0; i < N; i++) {
            __builtin_prefetch(&q[k + 16 * B]);
            k = q[k];
            //cerr << k << endl;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}