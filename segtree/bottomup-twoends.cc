#include "prefix.hh"

int t[2 * N];

void add(int k, int x) {
    k += N;
    t[k] += x;
    while (k > 1) {
        t[k>>1] += x;
        k >>= 1;
    }
}

int sum(int r) {
    r += N - 1;
    int l = N, s = 0;
    while (l <= r) {
        if ( l & 1) s += t[l++];
        if (~r & 1) s += t[r--];
        l >>= 1, r >>= 1;
    }
    return s;
}

/*
int sum(int k) {
    k += n;
    int res = t[k];
    while (k) {
        if ((k & 1) == 0)
            res += t[k - 1];
        k = (k - 1) / 2;
    }
    return res;
}
*/
