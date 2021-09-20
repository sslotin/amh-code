#include "inverse.hh"

int inverse(int _a) {
    long long a = _a;
    long long r = 1;
    int n = m - 2;
    while (n) {
        if (n & 1)
            r = r * a % m;
        a = a * a % m;
        n >>= 1;
    }
    //cout << _a << " " << r << " " << r * _a % m << endl;
    return r;
}
