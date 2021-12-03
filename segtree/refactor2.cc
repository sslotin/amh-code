#include "prefix.hh"
#include <x86intrin.h>

typedef int vec __attribute__ (( vector_size(32) ));

const int b = 4, B = (1 << b);

constexpr int round(int k) {
    return k / B * B; // (k & ~(B - 1));
}

constexpr int height(int m) {
    return (m == 0 ? 0 : height(m / B) + 1);
}

constexpr int offset(int h) {
    int res = 0;
    int m = N;
    while (h--) {
        res += round(m) + B;
        m /= B;
    }
    return res;
}

constexpr int h = height(N);
alignas(64) int t[offset(h)];
//int t[N * B / (B - 1)]; // +1?

struct Meta {
    alignas(64) int mask[B][B];

    constexpr Meta() : mask{} {
        for (int k = 0; k < B; k++)
            for (int i = 0; i < B; i++)
                mask[k][i] = (i > k ? -1 : 0);
    }
};

constexpr Meta T;

int sum(int k) {
    k += 1;
    int res = 0;
    for (int i = 0; i < h; i++)
        res += t[offset(i) + (k >> (i * b))];
    return res;
}

void add(int k, int x) {
    vec xx = x + vec{};
    for (int i = 0; i < h; i++) {
        vec *l = (vec*) (t + offset(i) + round(k));
        vec *m = (vec*) T.mask[k % B];
        for (int j = 0; j < B / 8; j++)
            l[j] += xx & m[j];
        k >>= b;
    }
}
