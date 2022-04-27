#include "inverse.hh"

typedef long long i64;

i64 binpow(i64 a, i64 n) {
    if (n == 0)
        return 1;
    if (n % 2 == 1)
        return 1ll * binpow(a, n - 1) * a % M;
    else {
        i64 b = binpow(a, n / 2) % M;
        return b * b % M;
    }
}

int inverse(int _a) {
    return binpow(_a, M - 2);
}
