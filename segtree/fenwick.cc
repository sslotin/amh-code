#include "prefix.hh"

int t[N + 1];

void add(int k, int x) {
    for (k += 1; k <= N; k += k & -k)
        t[k] += x;
}

int sum(int k) {
    int res = 0;
    for (; k != 0; k &= k - 1) // k -= k & -k
        res += t[k];
    return res;
}
