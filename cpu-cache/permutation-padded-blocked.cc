#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 16
#endif

const int M = N / 16; // # of memory accesses
const int K = (1<<25) / M;

struct padded_int {
    int val;
    int padding[15];
};

int p[M];
padded_int q[M];

int main() {
    iota(p, p + D - 1, 1);

    for (int i = 0; i + D < M; i += D) {
        random_shuffle(p, p + D - 1);
        int k = i;
        for (int j = 0; j < D - 1; j++) {
            q[k].val = i + p[j];
            k = i + p[j];
        }
        q[k].val = i + D;
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
        for (int i = 0; i < M; i++) {
            k = q[k].val;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / ((M / D) * D * K));

    return 0;
}