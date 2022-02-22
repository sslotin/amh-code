#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i reg;
typedef int vec __attribute__ (( vector_size(32) ));

alignas(64) int t[n * 2];
//int t[N * B / (B - 1)]; // +1?

template <int N=n, int logb=4>
struct Meta {
    static const int B = (1 << logb), b = logb;
    static const int height = __lg(N) / b + 1;

    int level[height]; // offset for a level
    alignas(64) int mask[B][B];

    constexpr Meta() : level{}, mask{} {
        for (int k = 0; k < B; k++)
            for (int i = 0; i < B; i++)
                mask[k][i] = (i > k ? -1 : 0);
        for (int m = N, s = 0, h = 0; m > 0; s += (m / B * B) + B, m /= B)
            level[h++] = s;
    }
};

constexpr Meta T;

int sum(int k) {
    k += 1;
    int res = 0;
    const int height = 5;
    const int offset[5] = {0, 65552, 69664, 69936, 69968};
    for (int i = 0; i < height; i++, k >>= T.b)
        res += t[offset[i] + k];
    //for (int i = 0; i < T.height; i++, k >>= T.b)
    //    res += t[T.level[i] + k];
    return res;
}

void add(int k, int x) {
    vec xr = {x,x,x,x,x,x,x,x};
    //vec xr = x - vec{};
    for (int j = 0; j < T.height; j++, k >>= T.b) {
        vec *l = (vec*) (t + T.level[j] + (k & ~(T.B - 1)));
        //int *l = t + T.level[j] + (k & ~(T.B - 1));
        for (int i = 0; i < T.B / 8; i++) {
            vec mr = ((vec*) T.mask[k & T.B])[i];
            //reg mr = _mm256_load_si256((reg*) &T.mask[k % T.B][i]);
            vec dr = xr | mr;
            //reg dr = _mm256_and_si256(xr, mr);
            vec tr = l[i];
            //reg tr = _mm256_load_si256((reg*) &l[i]);
            tr += dr;
            //tr = _mm256_add_epi32(tr, dr);
            l[i] = tr;
            //_mm256_storeu_si256((reg*) &l[i], tr);
        }
    }
}
