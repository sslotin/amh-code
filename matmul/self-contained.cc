#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,fma")

#include <bits/stdc++.h>
#include <x86intrin.h>

const int u = 48; // all block coordinates must be a multiple of lcm(6, 16) = 48

const int n = 40 * u; // = 1920 (also try 240, 480, 960)
alignas(64) float a[n * n], b[n * n], c[n * n];

typedef float vec __attribute__ (( vector_size(32) ));

// 6x16 micro-kernel
void kernel(int x, int y, int l, int r) {
    vec t[6][2]{0};

    for (int k = l; k < r; k++)
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 2; j++)
                t[i][j] += (vec{} + a[x * n + i * n + k]) * ((vec*) b)[n / 8 * k + y / 8 + j];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            ((vec*) c)[x * n / 8 + i * n / 8 + y / 8 + j] += t[i][j];
}

void matmul() {
    const int s3 = 2 * u;
    const int s2 = 4 * u;
    const int s1 = 8 * u;
    
    for (int i3 = 0; i3 < n; i3 += s3)
        for (int i2 = 0; i2 < n; i2 += s2)
            for (int i1 = 0; i1 < n; i1 += s1)
                for (int x = i2; x < std::min(i2 + s2, n); x += 6)
                    for (int y = i3; y < std::min(i3 + s3, n); y += 16)
                        kernel(x, y, i1, std::min(i1 + s1, n));
}

void naive() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                c[i * n + j] += a[i * n + k] * b[k * n + j];
}

void timeit(void (*f)()) {
    memset(c, 0, sizeof c);
    f();

    static const int idx[5] = {rand(), rand(), rand(), rand(), rand()};
    for (int i : idx)
        printf("%.4f ", c[i % (n * n)]);
    printf("\n");

    clock_t start = clock();

    int cnt = 0;
    while (clock() - start < CLOCKS_PER_SEC)
        f(), cnt++;

    printf("%.6f x %d\n", float(clock() - start) / CLOCKS_PER_SEC / cnt, cnt);
}

int main() {
    for (int i = 0; i < n * n; i++) {
        a[i] = float(rand()) / RAND_MAX;
        b[i] = float(rand()) / RAND_MAX;
    }

    timeit(naive);
    timeit(matmul);

    return 0;
}
