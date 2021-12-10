#include "stdio.h"
#include <algorithm>
#include <numeric>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

int p[N];

const int K = 2e7 / N;

struct node { node* ptr; };

node q[N];

int main() {
    iota(p, p + N, 0);
    random_shuffle(p, p + N);

    node* k = q + p[N - 1];

    for (int i = 0; i < N; i++)
        k = k->ptr = q + p[i];

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        for (int i = 0; i < N; i++) {
            k = k->ptr;
        }
        s ^= (long long) k->ptr;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}