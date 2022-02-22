#include "prefix.hh"

int t[2 * N];

void add(int k, int x) {
    k += N;
    while (k != 0) {
        t[k] += x;
        k >>= 1;
    }
}

int sum(int k) {
    int res = 0;
    k += N - 1;
    while (k != 0) {
        if (~k & 1)
            res += t[k];
        k = (k - 1) >> 1;
    }
    return res;
}
