#include "binsearch.hh"

int n;
int *_a;
int *t;

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
}

int lower_bound(int x) {
    int k = 1;
    while (k <= n) {
        //__builtin_prefetch(t + k * 32);
        
        //__builtin_prefetch(t + k * 64);
        //__builtin_prefetch(t + k * 64 + 16);
        //__builtin_prefetch(t + k * 64 + 32);
        //__builtin_prefetch(t + k * 64 + 48);

        //__builtin_prefetch_range();
        //__builtin_prefetch(t + k * 32);
        //__builtin_prefetch(t + k * 32 + 16);
        //__builtin_prefetch(t + k * 2 * B + B);
        k = 2 * k + (t[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return t[k];
}
