#include "prefix.hh"

const int last_layer = 1 << __lg(2 * N - 1);

int t[2 * N];

int leaf(int k) {
    k += last_layer;
    k -= (k >= 2 * N) * N;
    return k;
}

void add(int k, int x) {
    k = leaf(k);
    while (k != 0) {
        t[k] += x;
        k >>= 1;
    }
}

int sum(int k) {
    k = leaf(k - 1);
    int s = 0;
    while (k != 0) {
        s += ((k & 1) == 0) * t[k];
        k = (k - 1) >> 1;
    }
    return s;
}
