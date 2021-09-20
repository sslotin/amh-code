#include "prefix.hh"

int t[n];

void add(int k, int x) {
    for (; k < n; k = (k | (k+1)))
        t[k] += x;
}

int sum(int k) {
    int res = 0;
    for (; k >= 0; k = (k & (k + 1)) - 1)
        res += t[k];
    return res;
}

