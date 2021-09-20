#include "binpow.hh"

int binpow(int a, int n) {
    int r = 1;
    for (int i = 0; i < 30; i++) {
        //int t = (n >> i & 1 ? a : 1);
        r *= a;
        //int t = r * a;
        //int mask = (n >> i & 1 ? -1 : 0);
        //r = (~mask & r) | (mask & t);
        //r = (1 - c) * r + c * t;
        //r = (1 - c) * r + c * t;
        //r = ((n & (1<<i)) ? t : r);
        a = a * a % m;
    }
    return r;
}
