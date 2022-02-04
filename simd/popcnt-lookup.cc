#include "popcnt.hh"

struct Precalc {
    alignas(64) char counts[256];

    constexpr Precalc() : counts{} {
        for (int i = 0; i < 256; i++)
            counts[i] = __builtin_popcount(i);
    }
};

constexpr Precalc P;

int popcnt() {
    auto b = (unsigned char*) a;
    int res = 0;
    for (int i = 0; i < 4 * N; i++)
        res += P.counts[b[i]];
    return res;
}
