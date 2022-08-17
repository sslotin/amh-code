#include "permutation.hh"

int *p;
char *b;

void prepare(int n) {
    p = new int[n];
    b = new char[n];
    int k = 1, a = 5, c = 1;
    for (int i = 0; i < n; i++) {
        p[i] = k;
        k = (k * a + c) % n;
    }
}

typedef unsigned long long u64;

void permute(char *a, int n) {
    //for (int i = 0; i < n; i++)
    //    std::cout << int(a[i]) << std::endl;
    for (int i = 0; i < n; i += 8) {
        u64 x = 0;
        for (int j = 0; j < 8; j++) {
            //x |= ((u64) a[p[i + j]]) << (j * 8);
            x = (x << 8) | a[p[i + 7 - j]];
        }
        //for (int j = 0; j < 8; j++)
        //    std::cout << std::bitset<8>(a[p[i + j]]) << " ";
        //std::cout << std::endl << std::bitset<64>(x) << std::endl;
        *( (u64*) &b[i] ) = x;
        //for (int j = 0; j < 8; j++)
        //    std::cout << std::bitset<8>(b[i + j]) << " ";
        //std::cout << std::endl;
    }
    memcpy(a, b, n);
}
