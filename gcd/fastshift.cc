#include "gcd.hh"

int gcd(int a, int b) {
    if (!a || !b) return a | b;

    char az = __builtin_ctz(a);
    char bz = __builtin_ctz(b);
    char shift = min(az, bz);
    a >>= az;
    b >>= bz;
    
    while (true) {
        int dba = b - a;
        int dab = a - b;
        bz = __builtin_ctz(dba);
        if (dba == 0) break;
        int dbas = dba >> bz;
        int dabs = dab >> bz;
        a = (a < b ? a : b);
        b = (dba > dab ? dbas : dabs);
        //b >>= bz;
        //a = min(a, b);
        //b = max(dba, dab);
    }

    // if even:
    //   divide first by 2
    //   (try other powers?)
    // else:
    //   abs, min
    // blend
    
    return a << shift;
}
