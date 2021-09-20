#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i reg;

template <int N=n, int B=16>
struct Tree {
    alignas(64) int t[N * 2];
    //int t[N * B / (B - 1)]; // +1?
    alignas(64) int mask[B][B];

    // returns the index of starting element of a block
    static int round(int k) {
        return k / B * B; // k & ~(B - 1)
    }

    // constexpr?
    Tree() : t{}, mask{} {
        for (int k = 0; k < B; k++)
            for (int i = 0; i < B; i++)
                mask[k][i] = (i > k ? -1 : 0);
    }

    int sum(int k) {
        int res = 0;
        for (int m = N, s = 0; m > 0; s += round(m) + B, m /= B, k /= B)
            res += t[s + k];
        return res;
    }

    void add(int k, int x) {
        reg xr = _mm256_set1_epi32(x);
        for (int m = N, s = 0; m > 0; s += round(m) + B, m /= B, k /= B) {
            int l = round(k);
            //cerr << k << " " << x << " " << l << endl;
            for (int i = 0; i < B; i += 8) {
                reg mr = _mm256_load_si256((reg*) &mask[k % B][i]);
                reg dr = _mm256_and_si256(xr, mr);
                reg tr = _mm256_load_si256((reg*) &t[s + l + i]);
                tr = _mm256_add_epi32(tr, dr);
                _mm256_storeu_si256((reg*) &t[s + l + i], tr);
            }
        }
    }
};

Tree t;

void add(int k, int x) {
    t.add(k, x);
}

int sum(int k) {
    k += 1;
    return t.sum(k);
}
