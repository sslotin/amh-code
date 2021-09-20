#include "inverse.hh"

void f(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return;
    }
    bool s = false;
    if (a > b)
        swap(a, b), s = true;
    b -= a;
    char bz = __builtin_ctz(b);
    b >>= bz;
    f(a, b, x, y);
    x = x - (y >> bz);
    y = y >> bz;
    if (s)
        swap(x, y);
    // a x' + (b - a) / 2^k y' = 1  =>  a (x' - y' / 2^k) + b y' / 2^k = 1
}

int inverse(int a) {
    int _a = a;

    int b = m;

    char az = __builtin_ctz(a);

    int x = 1, y = 0;
    a >>= az;

    // bz = 0 on 1st iteration
    // 2^k a x + b y = 1  =>  x should be multiplied by 2^k
    // a, b = min(a, b), abs(a-b)  =>  
    // a(x+y) + (b-a)y = 1
    // b = 0  =>  x = 1, y = 0

    /*
    while (b) {
        //cerr << a << " " << b << endl;
        if (a > b) {
            swap(a, b);
            swap(x, y);
        }
        b -= a;
        x += y;
        char bz = __builtin_ctz(b);
        b >>= bz;
        y <<= bz;
    }*/

    f(a, b, x, y);

    // a / 2^k x' + b y' = 1  =>  
    x >>= az;
    
    cout << _a << " " << x << " " << (long long) _a * x % m << endl;

    return x;
}
