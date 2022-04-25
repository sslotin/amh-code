#include "inverse.hh"

int inverse(int _a) {
    long long a = _a, r = 1;
    
    #pragma GCC unroll(30)
    for (int l = 0; l < 30; l++) {
        if ( (M - 2) >> l & 1 )
            r = r * a % M;
        a = a * a % M;
    }

    return r;
}
