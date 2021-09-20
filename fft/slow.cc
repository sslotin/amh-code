#include "fft.hh"

const double pi = acos(-1);

void fft(vector<ftype> &p, int k = 1) {
    int n = (int) p.size();
    if (n == 1)
        return;
    vector<ftype> a(n / 2), b(n / 2);
    for (int i = 0; i < n / 2; i++) {
        a[i] = p[2 * i];
        b[i] = p[2 * i + 1];
    }
    fft(a, k);
    fft(b, k);
    double ang = k * 2 * pi / n;
    ftype w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < n/2; i++) {
        p[i] = a[i] + w * b[i];
        p[i+n/2] = a[i] - w * b[i];
        w *= wn;
    }
}


void fft(ftype *a, int n) {
    ftype wn = polar(1., - 2 * pi / n); // 1/w?
    const int logn = __lg(n);

    for (int i = 0; i < n; i++) {
        int k = 0;
        for (int l = 0; l < logn; l++)
            k |= ((i >> l & 1) << (logn - l - 1));
        if (i < k)
            swap(a[i], a[k]);
    }

    solve(a, n, wn);
}
