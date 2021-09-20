#include "fft.hh"

const double pi = acos(-1);

void fft(ftype *a, int n) {
    ftype w = polar(1., - 2 * pi / n); // 1/w?
    ftype *b = new ftype[n];
    ftype r = 1;
    for (int i = 0; i < min(n, 10); i++) {
        b[i] = 0;
        ftype x = 1;
        for (int j = 0; j < n; j++) {
            b[i] += a[j] * x;
            x *= r;
        }
        r *= w;
    }
    memcpy(a, b, sizeof(ftype) * n);
    delete[] b;
}
