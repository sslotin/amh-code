// Alligning arrays first

#include <bits/stdc++.h>

const int B = 8; // number of elements in a vector
const int vecsize = B * sizeof(float); // size of a vector in bytes
typedef float vector __attribute__ (( vector_size(vecsize) ));

vector* alloc(int n) {
    vector* ptr = (vector*) std::aligned_alloc(vecsize, vecsize * n);
    memset(ptr, 0, vecsize * n);
    return ptr;
}

float hsum(vector s) {
    float res = 0;
    for (int i = 0; i < B; i++)
        res += s[i];
    return res;
}

void matmul(const float *_a, const float *_b, float *c, int n) {
    int nB = (n + B - 1) / B;

    vector *a = alloc(n * nB);
    vector *b = alloc(n * nB);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * nB + j / 8][j % 8] = _a[i * n + j];
            b[i * nB + j / 8][j % 8] = _b[j * n + i]; // <- still transposed
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector s = {0};
            for (int k = 0; k < nB; k++)
                s += a[i * nB + k] * b[j * nB + k];
            c[i * n + j] = hsum(s);
        }
    }

    std::free(a);
    std::free(b);
}
