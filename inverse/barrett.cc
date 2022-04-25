#include "inverse.hh"

typedef unsigned long long u64;
typedef __uint128_t u128;

const u64 D = u64(-1) / M + 1;

u64 mul(u64 x, u64 y) {
    u64 p = x * y;
    u64 z = u128(p) * D >> 64;
    u64 t = p - z * M;
    return M <= t ? t + M : t;
}

int inverse(int _a) {
    u64 a = _a, r = 1;
    
    #pragma GCC unroll(30)
    for (int l = 0; l < 30; l++) {
        if ( (M - 2) >> l & 1 )
            r = mul(r, a);
        a = mul(a, a);
    }

    return r;
}
