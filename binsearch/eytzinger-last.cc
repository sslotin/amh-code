#include "binsearch.hh"
#include <bits/stdc++.h>

int n;
int *_a;
int *t;
int iters;

const int B = 16; // = 64 / 4 = cache_line_size / sizeof(int)

void eytzinger(int k = 1) {
    static int i = 0;
    if (k <= n) {
        eytzinger(2 * k);
        t[k] = _a[i++];
        eytzinger(2 * k + 1);
    }
}

void prepare(int *a, int _n) {
    n = _n;
    _a = a;
    t = (int*) std::aligned_alloc(64, 4 * (n + 1));
    eytzinger();

    t[0] = -1;
    iters = std::__lg(n + 1);
}

__attribute__((optimize("if-conversion2")))
int lower_bound(int x) {
    int k = 1;
    //#pragma GCC unroll 4
    for (int i = 0; i < iters; i++) {
        //__builtin_prefetch(t + k * B);
        k = 2 * k + (t[k] < x);
    }
    int *loc = (k <= n ? t + k : t);
    k = 2 * k + (*loc < x);
    k >>= __builtin_ffs(~k);
    return t[k];
}
