#include "prefix.hh"

inline int hole(int k) {
    return k + (k >> 10);
}

int t[2*n];

void add(int k, int x) {
    for (k += 1; k <= n; k += k & -k)
        t[hole(k)] += x;
}

int sum(int k) {
    int res = 0;
    for (k += 1; k > 0; k -= k & -k)
        res += t[hole(k)];
    return res;
}

