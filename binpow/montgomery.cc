#include "binpow.hh"

typedef __uint32_t u32;
typedef __uint64_t u64;
typedef __uint128_t u128;

struct montgomery {
    u32 n, nr;
    
    constexpr montgomery(u32 n) : n(n), nr(1) {
        for (int i = 0; i < 6; i++)
            nr *= 2 - n * nr;
    }

    u32 reduce(u64 x) const {
        u32 q = u32(x) * nr;
        u32 t = ((u64) q * n) >> 32;
        u32 xhi = (x >> 32);
        //if (xhi >= t)
            return (xhi - t);
        //else
        //    return (xhi - t) + n;
    }

    u32 mult(u32 x, u32 y) const {
        //return x * y;
        return reduce((u64) x * y);
    }

    u32 transform(u32 x) const {
        return (u64(x) << 32) % n;
    }
};

constexpr montgomery space(m);

int binpow(int _a, int n) {
    u32 a = _a;
    u32 r = space.transform(1);
    while (n) {
        //if (n & 1)
        //    r = space.mult(r, a);
        r += a;
        a = space.mult(a, a);
        n >>= 1;
    }
    return space.reduce(r);
}
