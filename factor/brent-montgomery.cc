#include "factor.hh"

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

    /*
    u64 reduce(u128 x) {
        u64 q = u64(x) * nr;
        u64 m = ((u128) q * n) >> 64;
        u64 xhi = (x >> 64);
        //cout << u64(x>>64) << " " << u64(x) << " " << q << endl;
        //cout << u64(m>>64) << " " << u64(m) << endl;
        //exit(0);
        if (xhi >= m)
            return (xhi - m);
        else
            return (xhi - m) + n;
    }
    */

    u64 mult(u64 x, u64 y) {
        return reduce((u128) x * y);
    }

    u64 transform(u64 x) {
        return (u128(x) << 64) % n;
    }
};

u64 mod(u64 x, u64 n) {
    return x >= n ? x - n : x;
}

u64 f(u64 x, u64 a, montgomery m) {
    // ??? nothing will break if result is 1 more, so no need for modulo
    u64 t = m.mult(x, x) + a;
    return t >= m.n ? t - m.n : t;
}

const int M = 256;

u64 rho(u64 n, u64 x0 = 2, u64 a = 1) {
    montgomery space(n);
    u64 x, y = space.transform(x0);
    a = space.transform(a);
    //cout << space.n << endl;
    for (int l = 1; true; l *= 2) {
        x = y;
        u64 m = 1;
        for (int i = 1; i <= l; i++) {
            //cout << y << endl;
            //cout << (y < n) << endl;
            y = f(y, a, space);
            //cout << y << endl;
            m = mod(space.mult(diff(x, y), m), n);
            //cout << m << endl;
            if (i % M == 0 || i == l) {
                if (m == 0) {
                    y = x;
                    while (true) {
                        y = f(y, a, space);
                        if (y == 0)
                            break;
                        u64 g = gcd(diff(x, y), n);
                        //cout << "! " << g << " " << x << " " << y << endl;
                        //cout << "# " << g << " " << space.reduce(x) << " " << space.reduce(y) << endl;
                        if (g != 1)
                            return g;
                    }
                } else {
                    u64 g = gcd(m, n);
                    if (g != 1)
                        return g;
                }
            }
        }
    }
}

u64 find_factor(u64 n) {
    return rho(n);
}
