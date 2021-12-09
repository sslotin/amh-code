#include "prefix.hh"

int t[4 * N];

void add(int k, int x) {
    int v = 1, l = 0, r = N;
    while (l + 1 < r) {
        t[v] += x;
        int m = (l + r) / 2;
        if (k < m)
            v = 2 * v, r = m;
        else
            v = 2 * v + 1, l = m;
    }
    t[v] += x;
}

int sum(int k) {
    if (k == N - 1)
        return t[1];
    int v = 1, l = 0, r = n;
    int s = 0;
    while (l < r) {
        int m = (l + r) / 2;
        v *= 2;
        if (k < m) {
            if (k == m - 1)
                return s + t[v];
            r = m;
        } else {
            s += t[v];
            v++;
            l = m;
        }
    }
    return s;
}
