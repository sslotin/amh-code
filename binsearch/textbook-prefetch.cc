#include "binsearch.hh"

int n;
int *t;

void prepare(int *a, int _n) {
    t = new int[n = _n];
    memcpy(t, a, 4 * n);
}

int lower_bound(int x) {
    int l = 0, r = n - 1;
    while (l < r) {
        int m = (l + r) / 2;
        __builtin_prefetch(&t[(l + m) >> 1]);
        __builtin_prefetch(&t[(m + 1 + r) >> 1]);
        if (t[m] >= x)
            r = m;
        else
            l = m + 1;
    }
    return t[l];
}