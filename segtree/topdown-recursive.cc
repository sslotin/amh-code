#include "prefix.hh"

int t[4 * N];

void _add(int k, int x, int v = 1, int l = 0, int r = N) {
    t[v] += x;
    if (l + 1 < r) {
        int m = (l + r) / 2;
        if (k < m)
            _add(k, x, 2 * v, l, m);
        else
            _add(k, x, 2 * v + 1, m, r);
    }
}

void add(int k, int x) {
    _add(k, x);
}

int _sum(int k, int v = 1, int l = 0, int r = N) {
    if (l >= k)
        return 0;
    if (r <= k)
        return t[v];
    int m = (l + r) / 2;
    return _sum(k, 2 * v, l, m)
         + _sum(k, 2 * v + 1, m, r);
}

int sum(int k) {
    return _sum(k);
}
