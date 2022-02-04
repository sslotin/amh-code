#include <bits/stdc++.h>
#include <x86intrin.h>

#ifndef N
#define N (1<<12)
#endif

#ifndef Q
#define Q (1<<26)
#endif

alignas(32) int a[N], q[Q];

typedef __m256i reg;

int hsum(__m256i x) {
    __m128i l = _mm256_extracti128_si256(x, 0);
    __m128i h = _mm256_extracti128_si256(x, 1);
    l = _mm_add_epi32(l, h);
    l = _mm_hadd_epi32(l, l);
    return _mm_extract_epi32(l, 0) + _mm_extract_epi32(l, 1);
}

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand();

    for (int i = 0; i < Q; i++)
        q[i] = rand() % N;

    reg s = _mm256_setzero_si256();

    clock_t start = clock();

    for (int i = 0; i < Q; i += 8) {
        reg idx = _mm256_load_si256( (reg*) &q[i] );
        reg x = _mm256_i32gather_epi32(a, idx, 4);
        s = _mm256_add_epi32(s, x);
    }

    int checksum = hsum(s);

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * Q / duration);

    printf("%d\n", checksum);

    return 0;
}
