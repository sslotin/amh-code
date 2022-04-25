#include "inverse.hh"

int inverse(int _a) {
    long long a = _a, r = 1;
    
    int n = M - 2;

    while (n) {
        if (n & 1)
            r = r * a % M;
        a = a * a % M;
        n >>= 1;
    }

    return r;
}
