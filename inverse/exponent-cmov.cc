#include "inverse.hh"

int inverse(int _a) {
    long long a = _a, r = 1;
    
    int n = M - 2;

    while (n) {
        long long b = a * a % M;
        long long t = r * a % M;
        r = (n & 1) ? t : r;
        a = b;
        n >>= 1;
    }

    return r;
}
