#include "factor.hh"

u64 find_factor(u64 n) {
    for (u64 d = 2; d * d <= n; d++)
        if (n % d == 0)
            return d;
    return 1;
}
