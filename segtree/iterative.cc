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
            res += t[k--]; // k = 1?
        k >>= 1;
    }
    return res;
}

/*
int sum(int r) {
    int l = 0;
    int ans = 0;
    l += n, r += n;
    while (l <= r) {
        if (l&1)    ans += t[l++];
        if (!(r&1)) ans += t[r--];
        l >>= 1, r >>= 1;
    }
    return ans;
}
*/

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
