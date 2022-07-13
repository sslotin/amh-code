#include "binsearch.hh"

int n;
int *t;

void prepare(int *a, int _n) {
    t = new int[n = _n];
    memcpy(t, a, 4 * n);
}

/*
int lower_bound(int x) {
    int *base = t, len = n;
    while (len > 1) {
        int half = len / 2;
        __builtin_prefetch(&base[(len - half) / 2]);
        __builtin_prefetch(&base[half + (len - half) / 2]);
        base = (base[half] < x ? &base[half] : base);
        len -= half;
    }
    return *(base + (*base < x));
}
*/

int lower_bound(int x) {
    int *base = t, len = n;
    while (len > 1) {
        __builtin_prefetch(&base[len / 4]);
        __builtin_prefetch(&base[3 * len / 4]);
        base += (base[(len - 1) / 2] < x) * (len / 2);
        len = (len + 1) / 2;
    }
    return *base;
}
