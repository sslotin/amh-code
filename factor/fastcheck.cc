#include "factor.hh"

const int N = (1 << 16);

struct Precalc {
    u64 magic[6542];

    constexpr Precalc() : magic{} {
        bool marked[N] = {};
        int n_primes = 0;

        for (int i = 2; i < N; i++) {
            if (!marked[i]) {
                magic[n_primes++] = u64(-1) / i + 1;
                for (int j = 2 * i; j < N; j += i)
                    marked[j] = true;
            }
        }
    }
};

constexpr Precalc P{};

/*
const u64 d = 1e9 + 7;
const u64 m = u64(-1) / d + 1;
const u64 r = u64(-1) / m + 1;
static_assert(d == r);
*/

u64 find_factor(u64 n) {
    for (u64 m : P.magic)
        if (m * n < m)
            return u64(-1) / m + 1;
    return 1;
}
