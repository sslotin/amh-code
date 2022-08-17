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

void permute(char *a, int n) {
    for (int i = 0; i < n; i++)
        //b[p[i]] = a[i]; // 2x slower
        b[i] = a[p[i]];
    memcpy(a, b, n);
}
