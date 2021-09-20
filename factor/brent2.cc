#include "factor.hh"

u64 f(u64 x, u64 a, u64 mod) {
    return ((u128) x * x + a) % mod;
}

const int M = 256;

u64 rho(u64 n, u64 x0 = 2, u64 a = 1) {
    u64 x = x0, y = x0;
    for (int l = 1; true; l *= 2) {
        x = y;
        u64 m = 1;
        for (int i = 1; i <= l; i++) {
            y = f(y, a, n);
            m = (u128) (x > y ? x - y : y - x) * m % n;
            if (i % M == 0 || i == l) {
                if (m == 0) {
                    y = x;
                    while (true) {
                        y = f(y, a, n);
                        u64 g = gcd((x > y ? x - y : y - x), n);
                        if (g != 1)
                            return g;
                    }
                }
                u64 g = gcd(m, n);
                if (g != 1)
                    return g;
            }
        }
    }
}

u64 find_factor(u64 n) {
    return rho(n);
}
