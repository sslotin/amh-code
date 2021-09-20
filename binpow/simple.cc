#include "binpow.hh"

int binpow(int a, int n) {
    int r = 1;
    while (n) {
        if (n & 1)
            r = r * a % m;
        a = a * a % m;
        n >>= 1;
    }
    return r;
}
