#include "inverse.hh"

int inverse(int a) {
    int b = M, s = 1, t = 0;
    while (true) {
        if (a == 1)
            return s;
        t -= b / a * s;
        b %= a;
        if (b == 1)
            return t + M;
        s -= a / b * t;
        a %= b;
    }
}
