#include "gcd.hh"

int gcd(int a, int b) {
    if (!a || !b) return a | b;

    char az = __builtin_ctz(a);
    char bz = __builtin_ctz(b);
    char shift = min(az, bz);
    a >>= az;
    
    while (true) {
        b >>= bz;
        int dba = b - a;
        int dab = a - b;
        bz = __builtin_ctz(dba);
        if (dba == 0) break;
        a = min(a, b);
        b = max(dba, dab);
    }
    
    return a << shift;
}
