#include "factor.hh"

u64 f(u64 x, u64 a, u64 mod) {
    return ((u128) x * x + a) % mod;
}

u64 diff(u64 a, u64 b) {
    return a > b ? a - b : b - a;
}

const int N = (1 << 20);
u64 x[N];

u64 rho(u64 n, u64 x0 = 2, u64 a = 1) {
    x[0] = x0;

    for (int i = 1; i < N; i++) {
        x[i] = f(x[i - 1], a, n);
        if (i % 2 == 0) {
            u64 g = gcd(diff(x[i], x[i >> 1]), n);
            if (g != 1)
                return g;
        }
    }

    return 1;
}

u64 find_factor(u64 n) {
    return rho(n);
}
