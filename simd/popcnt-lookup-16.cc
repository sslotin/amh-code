#include "popcnt.hh"

struct Precalc {
    alignas(64) char counts[1<<16];

    constexpr Precalc() : counts{} {
        for (int i = 0; i < (1<<16); i++)
            counts[i] = __builtin_popcount(i);
    }
};

constexpr Precalc P;

int popcnt() {
    auto b = (unsigned short*) a;
    int res = 0;
    for (int i = 0; i < 2 * N; i++)
        res += P.counts[b[i]];
    return res;
}
