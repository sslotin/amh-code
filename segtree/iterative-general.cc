#include "prefix.hh"

const int last_layer = 1 << __lg(2 * N - 1);

int t[2 * N];

int leaf(int k) {
    k += last_layer;
    k -= (k >= 2 * N) * N;
    return k;
    /*
    k += last_layer;
    if (k >= 2 * N)
        k += (N - last_layer);
    return k;
    */
}

void add(int k, int x) {
    /*
    cout << N << " " << last_layer << endl;
    for (int i = 0; i < N; i++)
        cout << i << " " << leaf(i) << " " << bitset<5>(leaf(i)) << endl;
    exit(0);
    */
    k = leaf(k);
    while (k != 0) {
        t[k] += x;
        k >>= 1;
    }
}

int sum(int k) {
    k = leaf(k - 1);
    int res = 0;
    while (k != 0) {
        if (~k & 1)
            res += t[k];
        k = (k - 1) / 2;
    }
    return res;
}
