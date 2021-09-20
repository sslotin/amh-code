#include "factor.hh"

template<int N = (1<<16)>
struct Precalc {
    char divisor[N];

    constexpr Precalc() : divisor{} {
        for (int i = 0; i < N; i++)
            divisor[i] = 1;
        for (int i = 2; i * i < N; i++)
            if (divisor[i] == 1)
                for (int k = i * i; k < N; k += i)
                    divisor[k] = i;
    }
};

constexpr Precalc precalc{};

u64 find_factor(u64 n) {
    return precalc.divisor[n];
}
