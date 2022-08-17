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
    const int K = 4;
    for (int i = 0; i < n; i += 8 * K) {
        u64 x[K] = {0};
        for (int k = 0; k < K; k++) {
            for (int j = 0; j < 8; j++) {
                x[k] |= ((u64) a[p[i + k * 8 + j]]) << (j * 8);
                //x[k] = (x[k] << 8) | a[p[i + k * 8 + 7 - j]];
            }
            *( (u64*) &b[i + k * 8] ) = x[k];
        }
    }
    memcpy(a, b, n);
}
