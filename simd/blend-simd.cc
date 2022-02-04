#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

const int K = 1e10 / N;

typedef __m256i reg;

alignas(32) int a[N];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    clock_t start = clock();

    const reg c = _mm256_set1_epi32(49);
    const reg z = _mm256_setzero_si256();
    reg s = _mm256_setzero_si256();

    for (int t = 0; t < K; t++) {
        __sync_synchronize();
        for (int i = 0; i < N; i += 8) {
            reg x = _mm256_load_si256( (reg*) &a[i] );
            reg mask = _mm256_cmpgt_epi32(x, c);
            x = _mm256_blendv_epi8(x, z, mask);
            s = _mm256_add_epi32(s, x);
        }
    }

    int s_val = 0;
    for (int i = 0; i < 8; i++)
        s_val += _mm256_extract_epi32(s, i);

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * N * K / duration);

    printf("%d\n", s_val);

    return 0;
}
