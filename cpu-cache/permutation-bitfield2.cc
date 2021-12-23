#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

int p[N];

const int K = 2e7 / N;

struct __attribute__ ((packed)) node { int idx : 24; };

node q[N + 1];

int main() {
    iota(p, p + N, 0);
    random_shuffle(p, p + N);

    int k = p[N - 1];

    for (int i = 0; i < N; i++) {
        k = q[k].idx = p[i];
    }

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        for (int i = 0; i < N; i++) {
            //__builtin_prefetch(&a[i + 10 * D]);
            //cerr << k << endl;
            //cerr << (q + k) << endl;
            //cerr << q[k].idx << endl;
            //int *w = (int*) (q + k);
            //cerr << w << endl;
            //cerr << *w << endl;
            k = *((int*) (q + k));
            //cerr << k << endl;
            k &= ((1<<24) - 1);
            //cerr << k << endl;
            //k = q[k].idx;
            //cerr << k << endl;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    printf("%ld\n", sizeof q);

    return 0;
}