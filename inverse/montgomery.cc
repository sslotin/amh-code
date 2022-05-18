#include "inverse.hh"
#include <x86intrin.h>

typedef __uint32_t u32;
typedef __uint64_t u64;
typedef __uint128_t u128;

typedef __m256i v8i;

struct montgomery {
    u32 n, nr, r2;
    
    constexpr montgomery(u32 n) : n(n), nr(1), r2((u128(1) << 64) % n) {
        for (int i = 0; i < 6; i++)
            nr *= 2 - n * nr;
    }

    /*
    u32 reduce(u64 x) const {
        u32 q = u32(x) * nr;
        u64 m = (u64) q * n;
        u32 y = ((x - m) >> 32);
        return x < m ? y + n : y;
    }
    */

    u32 reduce(u64 x) const {
        u32 q = u32(x) * nr;
        u32 m = ((u64) q * n) >> 32;
        //std::cout << xhi << " " << (xhi >= m) << std::endl;
        return (x >> 32) + n - m;
        //u32 t = xhi - m;
        //return xhi >= m ? t : t + n;
    }

    //v8i multiply(v8i x, v8i y) const {   
    //    v8i = _mm256_mullo_epi32
    //}

    u32 multiply(u32 x, u32 y) const {
        return reduce((u64) x * y);
    }

    u32 transform(u32 x) const {
        return reduce(u64(x) << 32);
    }
};

/*
struct montgomery {
    u64 n, nr;
    
    constexpr montgomery(u64 n) : n(n), nr(1) {
        for (int i = 0; i < 6; i++)
            nr *= 2 - n * nr;
    }

    u64 reduce(u128 x) const {
        u64 q = u64(x) * nr;
        u64 m = ((u128) q * n) >> 64;
        u64 xhi = (x >> 64);
        //std::cout << xhi << " " << (xhi >= m) << std::endl;
        return xhi + n - m;
        //u32 t = xhi - m;
        //return xhi >= m ? t : t + n;
    }

    //v8i multiply(v8i x, v8i y) const {   
    //    v8i = _mm256_mullo_epi32
    //}

    u64 multiply(u64 x, u64 y) const {
        return reduce((u128) x * y);
    }

    u64 transform(u64 x) const {
        return (u128(x) << 64) % n;
    }
};
*/

constexpr montgomery space(M);

/*
int inverse(int _a) {
    u32 a = space.transform(_a);
    u32 r = space.transform(1);
    
    int n = M - 2;
    while (n) {
        if (n & 1)
            r = space.multiply(r, a);
        a = space.multiply(a, a);
        n >>= 1;
    }
    
    return space.reduce(r);
}
*/

int inverse(int _a) {
    u32 a = space.transform(_a);
    u32 r = space.transform(1);
    
    #pragma GCC unroll(30)
    for (int l = 0; l < 30; l++) {
        if ( (M - 2) >> l & 1 )
            r = space.multiply(r, a);
        a = space.multiply(a, a);
    }

    return space.reduce(r);
}
