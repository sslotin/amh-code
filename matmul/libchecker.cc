#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("avx2")

#include <bits/stdc++.h>
#include <x86intrin.h>

typedef __m256i reg;
typedef unsigned long long u64;

const u64 mod = 998244353;

const int N = 1056; // 48 * 22, divisible by 2^5
alignas(64) u64 a[N][N], b[N][N], c[N][N];

// 6x8x16
void kernel(int x, int y, int l) {
    //std::cerr << x << " " << y << " " << l << std::endl;
    reg t[6][2];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            t[i][j] = _mm256_load_si256((reg*) &c[x + i][y + 4 * j]);

    for (int k = l; k < l + 96; k++) {
        reg v1 = _mm256_load_si256((reg*) &b[k][y]);
        reg v2 = _mm256_load_si256((reg*) &b[k][y + 4]);
        for (int i = 0; i < 6; i++) {
            reg alpha = (reg) _mm256_broadcast_pd((__m128d*) &a[x + i][k]);
            reg a1 = _mm256_mul_epu32(alpha, v1);
            reg a2 = _mm256_mul_epu32(alpha, v2);
            t[i][0] = _mm256_add_epi64(t[i][0], a1);
            t[i][1] = _mm256_add_epi64(t[i][1], a2);
        }
    }

    //std::cerr << "here" << std::endl;

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            _mm256_store_si256((reg*) &c[x + i][y + 4 * j], t[i][j]);

    //for (int i = 0; i < 6; i++)
    //    for (int j = 0; j < 8; j++)
    //        c[x + i][y + j] %= mod;
}

const int SIZE = (1<<14);
char buf[SIZE];
int buf_pos = SIZE; 

char read_char() {
    if (buf_pos == SIZE) {
        fread(buf, 1, SIZE, stdin);
        buf_pos = 0;
    }
    return buf[buf_pos++];
}

int read_int() {
    int x = 0;

    while (true) {
        char c = read_char();
        if (c >= '0' && c <= '9')
            x = 10 * x + (c - '0');
        else
            return x;
    }
}

int main() {
    int n = read_int(), m = read_int(), k = read_int();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            a[i][j] = read_int();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            b[i][j] = read_int();
    
    /*
    for (int x = 0; x < N; x += 6)
        for (int y = 0; y < N; y += 8)
            for (int l = 0; l < N; l += 16)
                kernel(x, y, l);
    */

    const int s3 = 1056 / 22;
    const int s2 = 1056 / 8;

    for (int i3 = 0; i3 < N; i3 += s3)
        for (int i2 = 0; i2 < N; i2 += s2)
            for (int i1 = 0; i1 < N; i1 += 96)
                for (int x = i2; x < i2 + s2; x += 6)
                    for (int y = i3; y < i3 + s3; y += 8)
                        kernel(x, y, i1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", c[i][j]);
        printf("\n");
    }

    return 0;
}
