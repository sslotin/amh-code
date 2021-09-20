#include <bits/stdc++.h>
using namespace std;

void checkmod(uint32_t n, uint32_t d) {
    uint64_t c = UINT64_C(0xFFFFFFFFFFFFFFFF) / d + 1;

    uint64_t lowbits = c * n;
    uint64_t res = ((__uint128_t) lowbits * d) >> 64; 
    
    if (res != (n % d)) {
        cout << n << " " << d << endl;
        exit(0);
    }
}

void checkdiv(uint32_t n, uint32_t d) {
    uint64_t c = UINT64_C(0xFFFFFFFFFFFFFFFF) / d + 1;

    uint64_t res = ((__uint128_t) c * n) >> 64;
        
    if (res != (n / d)) {
        cout << n << " " << d << " : " << n / d << " " << res << endl;
        cout << "WA" << endl;
        exit(0);
    }
}

int main() {
    int i = 0;
    
    while (true) {
        if (++i % 1'000'000 == 0)
            cout << i << endl;
        uint64_t n = rand();
        uint32_t d = rand() % 100;
        if (d > 1)
            checkdiv(n, d);
    }

    return 0;
}