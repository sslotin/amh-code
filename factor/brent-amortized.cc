#include "factor.hh"

u64 f(u64 x, u64 a, u64 mod) {
    return ((u128) x * x + a) % mod;
}

u64 diff(u64 a, u64 b) {
    return a > b ? a - b : b - a;
}

const int M = 1024;

u64 rho(u64 n, u64 x0 = 2, u64 a = 1) {
    u64 x = x0, y = x0, p = 1;
    
    for (int l = M; l < (1 << 20); l *= 2) {
        x = y;
        for (int i = 0; i < l; i += M) {
            for (int j = 0; j < M; j++) {
                y = f(y, a, n);
                p = (u128) p * diff(x, y) % n;
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
