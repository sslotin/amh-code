#include "prefix.hh"

int t[2 * n];

void add(int k, int x) {
    k += n;
    t[k] += x;
    while (k > 1) {
        t[k>>1] += x;
        k >>= 1;
    }
}

int sum(int k) {
    int res = 0;
    k += n;
    while (k != 0) {
        if (!(k&1))
            res += t[k]; // k = 1?
        k = (k - 1) / 2;
    }
    return res;
}
