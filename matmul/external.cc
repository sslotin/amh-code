// Micro-kernel

#include <bits/stdc++.h>

void mul(const float *a, const float *b, float *c, int n, int N) {
    if (n <= 32) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    c[i * N + j] += a[i * N + k] * b[k * N + j];
    } else {
        int k = n / 2;

        // c11 = a11 b11 + a12 b21
        mul(a,     b,         c, k, N);
        mul(a + k, b + k * N, c, k, N);
        
        // c12 = a11 b12 + a12 b22
        mul(a,     b + k,         c + k, k, N);
        mul(a + k, b + k * N + k, c + k, k, N);
        
        // c21 = a21 b11 + a22 b21
        mul(a + k * N,     b,         c + k * N, k, N);
        mul(a + k * N + k, b + k * N, c + k * N, k, N);
        
        // c22 = a21 b12 + a22 b22
        mul(a + k * N,     b + k,         c + k * N + k, k, N);
        mul(a + k * N + k, b + k * N + k, c + k * N + k, k, N);

        if (n & 1) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    for (int k = (i < n - 1 && j < n - 1) ? n - 1 : 0; k < n; k++)
                        c[i * N + j] += a[i * N + k] * b[k * N + j];
        }
    }
}

void matmul(const float *a, const float *b, float *c, int n) {
    //float *b = new float[n * n];

    /*
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            b[i * n + j] = _b[j * n + i];
    */

    mul(a, b, c, n, n);

    //delete[] b;
}
