#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

int p[15], q[N];

const int K = 2e7 / N;

int main() {
    iota(p, p + 15, 1);

    for (int i = 0; i + 16 < N; i += 16) {
        random_shuffle(p, p + 15);
        int k = i;
        for (int j = 0; j < 15; j++) {
            q[k] = i + p[j];
            k = i + p[j];
        }
        q[k] = i + 16;
    }

    /*
    for (int i = 0; i < 15; i++)
        cerr <<  i << " " << p[i] << endl;

    for (int i = 0; i < N; i++)
        cerr <<  i << " " << q[i] << endl;
    */

    clock_t start = clock();

    volatile int s = 0;
    int k = 0;

    for (int t = 0; t < K; t++) {
        for (int i = 0; i < N; i++) {
            //__builtin_prefetch(&a[i + 10 * D]);
            k = q[k];
            //cerr << k << endl;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}