#include "inverse.hh"

typedef __uint32_t u32;
typedef __uint64_t u64;
typedef __uint128_t u128;

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
        u64 t = xhi - m;
        return xhi >= m ? t : t + n;
        /*
        if (xhi >= m)
            return (xhi - m);
        else
            return (xhi - m) + n;
        */
    }

    u64 mult(u64 x, u64 y) const {
        return reduce((u128) x * y);
    }

    u64 transform(u64 x) const {
        return (u128(x) << 64) % n;
    }
};

constexpr montgomery space(m);

int inverse(int _a) {
    //u64 a = _a, r = 1;
    u64 a = space.transform(_a);
    u64 r = space.transform(1);
    int n = m - 2;
    while (n) {
        if (n & 1)
            r = space.mult(r, a);
        a = space.mult(a, a);
        n >>= 1;
    }
    //cout << _a << " " << r << " " << r * _a % m << endl;
    return space.reduce(r);
}
