#include "binpow.hh"

uint64_t c = uint64_t(-1) / m + 1; // ceil(2^64 / d)

uint64_t mod(uint32_t x) {
    uint64_t lowbits = c * x;
    uint64_t res = ((__uint128_t) lowbits * m) >> 64; 

    return res;
}

int binpow(int a, int n) {
    int r = 1;
    while (n) {
        if (n & 1)
            r = mod(r * a);
        a = mod(a * a);
        n >>= 1;
    }
    return r;
}
