#include <bits/stdc++.h>
#include "matmul.hh"

using namespace std;

const float EPS = 1e-5;

void baseline(const float *a, const float *b, float *c, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                c[i * n + j] += a[i * n + k] * b[k * n + j];
}

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 256);

    float *a = new float[n * n];
    float *b = new float[n * n];
    for (int i = 0; i < n * n; i++) {
        a[i] = float(rand()) / RAND_MAX;
        b[i] = float(rand()) / RAND_MAX;
    }

    float *c1 = new float[n * n];
    float *c2 = new float[n * n];

    memset(c1, 0, 4 * n * n);
    memset(c2, 0, 4 * n * n);

    baseline(a, b, c1, n);
    matmul(a, b, c2, n);

    for (int i = 0; i < n * n; i++) {
        float err = abs(c1[i] - c2[i]);
        assert(err/abs(c2[i]) < EPS);
    }

    printf("OK\n");

    return 0;
}
