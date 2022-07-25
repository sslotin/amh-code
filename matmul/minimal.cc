#include <bits/stdc++.h>
#include <x86intrin.h>

void matmul(const float *_a, const float *_b, float *_c, int n) {
    const int MAXN = 1920 * 1920; // ~15MB each
    alignas(64) static float a[MAXN], b[MAXN], c[MAXN];

    memset(c, 0, 4 * n * n);

    for (int i = 0; i < n; i++) {
        memcpy(&a[i * n], &_a[i * n], 4 * n);
        memcpy(&b[i * n], &_b[i * n], 4 * n);
    }

    for (int i3 = 0; i3 < n; i3 += 96)
        for (int i2 = 0; i2 < n; i2 += 192)
            for (int i1 = 0; i1 < n; i1 += 384)
                for (int x = i2; x < i2 + 192; x += 6)
                    for (int y = i3; y < i3 + 96; y += 16)
                        for (int k = i1; k < i1 + 384; k++)
                            for (int i = 0; i < 6; i++)
                                for (int j = 0; j < 2; j++)
                                    ((__m256*) c)[x * n / 8 + i * n / 8 + y / 8 + j]
                                    += (__m256{} + a[x * n + i * n + k])
                                       * ((__m256*) b)[n / 8 * k + y / 8 + j];
    for (int i = 0; i < n; i++)
        memcpy(&_c[i * n], &c[i * n], 4 * n);
    
    /*
    delete[] a;
    delete[] b;
    delete[] c;
    */
}
