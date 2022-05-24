#include "factor.hh"

u64 gcd(long long a, long long b) {
    if (a == 0) return b;
    if (b == 0) return a;

    long long az = __builtin_ctz(a);
    long long bz = __builtin_ctz(b);
    long long shift = az < bz ? az : bz;
    b >>= bz;
    
    while (a != 0) {
        a >>= az;
        long long diff = b - a;
        az = __builtin_ctz(diff);
        b = a < b ? a : b;
        a = a < b ? diff : -diff;
    }
    
    return b << shift;
}

u64 diff(u64 x, u64 y) {
    return (x > y ? x - y : y - x);
}

struct montgomery {
    u64 n, nr;
    
    montgomery(u64 n) : n(n) {
        nr = 1;
        for (int i = 0; i < 6; i++)
            nr *= 2 - n * nr;
    }

    u64 reduce(u128 x) const {
        u64 q = u64(x) * nr;
        u64 m = ((u128) q * n) >> 64;
        return (x >> 64) + n - m;
    }

    u64 multiply(u64 x, u64 y) {
        return reduce((u128) x * y);
    }

    u64 transform(u64 x) {
        return (u128(x) << 64) % n;
    }

    u64 mod(u64 x) {
        return x >= n ? x - n : x;
    }
};

u64 f(u64 x, u64 a, montgomery m) {
    // ??? nothing will break if result is 1 more, so no need for modulo
    return m.multiply(x, x) + a;
}

const int M = 1024;

u64 rho(u64 n, u64 x0 = 2, u64 a = 1) {
    montgomery m(n);
    u64 y = x0;
    
    for (int l = M; l < (1 << 20); l *= 2) {
        u64 x = y, p = 1;
        for (int i = 0; i < l; i += M) {
            for (int j = 0; j < M; j++) {
                y = f(y, a, m);
                p = m.multiply(p, diff(x, y));
            }
            if (u64 g = gcd(p, n); g != 1)
                return g;
        }
    }

    return 1;
}

u64 find_factor(u64 n) {
    return rho(n);
}
