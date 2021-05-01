// Micro-kernel

#include <bits/stdc++.h>
#include <x86intrin.h>

const int B = 8; // number of elements in a vector
typedef float vector __attribute__ (( vector_size(4 * B) ));

float* alloc(int n) {
    float* ptr = (float*) std::aligned_alloc(64, 4 * n);
    memset(ptr, 0, 4 * n); 
    return ptr;
}

// rec. throughput of FMA is ½
// latency is 5
// we need at least 5 * 2 = 10 registers to keep pipeline running

// c: 12 x 8
// a: 12 x k
// b:  k x 8

// c: 6 x 16
// a: 6 x k
// b: k x 16
// c[x:x+6][y:y+16] += a[x:x+6][l:r] * b[l:r][y:y+16]
void kernel(float *a, vector *b, vector *c, int x, int y, int l, int r, int n) {
    vector t[6][2] = { _mm256_setzero_ps() };

    for (int k = l; k < r; k++) {
        for (int i = 0; i < 6; i++) {
            vector alpha = _mm256_set1_ps(a[(x + i) * n + k]);
            for (int j = 0; j < 2; j++)
                t[i][j] += alpha * b[(k * n + y) / 8 + j];
        }
    }

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            c[((x + i) * n + y) / 8 + j] += t[i][j];
}

void matmul(const float *_a, const float *_b, float *_c, int n) {
    int nx = (n + 5) / 6 * 6;
    int ny = (n + 15) / 16 * 16;
    
    float *a = alloc(nx * ny);
    float *b = alloc(nx * ny);
    float *c = alloc(nx * ny);

    for (int i = 0; i < n; i++) {
        memcpy(&a[i * ny], &_a[i * n], 4 * n);
        memcpy(&b[i * ny], &_b[i * n], 4 * n);
    }

    for (int x = 0; x < nx; x += 6)
        for (int y = 0; y < ny; y += 16)
            kernel(a, (vector*) b, (vector*) c, x, y, 0, n, ny);

    for (int i = 0; i < n; i++)
        memcpy(&_c[i * n], &c[i * ny], 4 * n);
    
    delete[] a;
    delete[] b;
    delete[] c;
}
