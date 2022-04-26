#include "inverse.hh"

int inverse(int a) {
    int b = M, x = 1, y = 0;
    while (true) {
        if (a == 1)
            return x;
        y -= b / a * x;
        b %= a;
        
        if (b == 1)
            return y + M;
        x -= a / b * y;
        a %= b;
    }
}
