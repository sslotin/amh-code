#include "factor.hh"

struct Montgomery {
    u64 n, nr;
    
    Montgomery(u64 n) : n(n) {
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

u64 f(u64 x, u64 a, Montgomery m) {
    return m.mod(m.multiply(x, x) + a);
}

u64 diff(u64 a, u64 b) {
    return a > b ? a - b : b - a;
}

const int N = (1 << 20), M = 512;
u64 x[N];

u64 rho(u64 n, u64 x0 = 5432, u64 a = 1) {
    Montgomery space(n);
    x[0] = x0;
    // transform x0 and a?

    u64 m = 1;

    for (int i = 1; i < N; i++) {
        x[i] = f(x[i - 1], a, space);
        if (i % 2 == 0) {
            m = space.multiply(m, diff(x[i], x[i >> 1]));
            if (i % M == 0) {
                if (m == 0) {
                    while (true) {
                        u64 g = gcd(diff(x[i], x[i >> 1]), n);
                        if (g != 1)
                            return g;
                        i -= 2;
                    }
                }
                u64 g = gcd(m, n);
                if (g != 1)
                    return g;
            }
        }
    }

    return 1;
}

u64 find_factor(u64 n) {
    return rho(n);
}
