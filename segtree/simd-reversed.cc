#include "prefix.hh"
#include <x86intrin.h>

typedef int vec __attribute__ (( vector_size(32) ));

const int b = 4, B = (1 << b);

constexpr int round(int k) {
    return k & ~(B - 1); // = k / B * B
}

constexpr int height(int n) {
    return (n <= B ? 1 : height(n / B) + 1);
}

constexpr int offset(int h) {
    int s = 0, n = N;
    while (h--) {
        s += (n + B - 1) / B * B;
        n /= B;
    }
    return s;
}

constexpr int H = height(N);
alignas(64) int t[offset(H)];

struct Precalc {
    alignas(64) int mask[B][B];

    constexpr Precalc() : mask{} {
        for (int k = 0; k < B; k++)
            for (int i = 0; i < B; i++)
                mask[k][i] = (i > k ? -1 : 0);
    }
};

constexpr Precalc T;

int sum(int k) {
    int res = 0;
    for (int h = H - 1; h >= 0; h--)
        res += t[offset(h) + (k >> (h * b))];
    return res;
}

void add(int k, int _x) {
    vec x = _x + vec{};
    for (int h = H - 1; h >= 0; h--) {
        int p = k >> (h * b);
        auto l = (vec*) &t[offset(h) + (p & ~(B - 1))];
        auto m = (vec*) T.mask[p % B];
        for (int i = 0; i < B / 8; i++)
            l[i] += x & m[i];
    }
}
