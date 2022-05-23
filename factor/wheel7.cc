#include "factor.hh"

u64 find_factor(u64 n) {
    for (u64 d : {2, 3, 5})
        if (n % d == 0)
            return d;
    u64 increments[] = {0, 4, 6, 10, 12, 16, 22, 24};
    for (u64 d = 7; d * d <= n; d += 30) {
        for (u64 k = 0; k < 8; k++) {
            u64 x = d + increments[k];
            if (n % x == 0)
                return x;
        }
    }
    return 1;
}