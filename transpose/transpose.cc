#include <bits/stdc++.h>
#include <x86intrin.h>

typedef unsigned char u8;
typedef __m256i reg;

alignas(64) u8 a[256], b[256];

int main() {
    for (int i = 0; i < 256; i++)
        a[i] = i;

    reg r1 = _mm256_load_si256((reg*) &a[0]);
    reg r2 = _mm256_load_si256((reg*) &a[32]);
    reg r3 = _mm256_load_si256((reg*) &a[64]);
    reg r4 = _mm256_load_si256((reg*) &a[96]);
    reg r5 = _mm256_load_si256((reg*) &a[128]);
    reg r6 = _mm256_load_si256((reg*) &a[160]);
    reg r7 = _mm256_load_si256((reg*) &a[192]);
    reg r8 = _mm256_load_si256((reg*) &a[224]);

    reg d1 = _mm256_unpacklo_epi8(r1, r2);
    reg d2 = _mm256_unpackhi_epi8(r1, r2);
    reg d3 = _mm256_unpacklo_epi8(r3, r4);
    reg d4 = _mm256_unpackhi_epi8(r3, r4);
    reg d5 = _mm256_unpacklo_epi8(r5, r6);
    reg d6 = _mm256_unpackhi_epi8(r5, r6);
    reg d7 = _mm256_unpacklo_epi8(r7, r8);
    reg d8 = _mm256_unpackhi_epi8(r7, r8);

    reg q1 = _mm256_unpacklo_epi8(d1, d3);
    reg q2 = _mm256_unpackhi_epi8(d1, d3);
    reg q3 = _mm256_unpacklo_epi8(d2, d4);
    reg q4 = _mm256_unpackhi_epi8(d2, d4);
    reg q5 = _mm256_unpacklo_epi8(d5, d7);
    reg q6 = _mm256_unpackhi_epi8(d5, d7);
    reg q7 = _mm256_unpacklo_epi8(d6, d8);
    reg q8 = _mm256_unpackhi_epi8(d6, d8);

    r1 = _mm256_unpacklo_epi8(q1, q5);
    r2 = _mm256_unpacklo_epi8(q2, q6);
    r3 = _mm256_unpacklo_epi8(q3, q7);
    r4 = _mm256_unpacklo_epi8(q4, q8);
    r5 = _mm256_unpackhi_epi8(q1, q5);
    r6 = _mm256_unpackhi_epi8(q2, q6);
    r7 = _mm256_unpackhi_epi8(q3, q7);
    r8 = _mm256_unpackhi_epi8(q4, q8);
    
    _mm256_store_si256((reg*) &a[0], r1);
    _mm256_store_si256((reg*) &a[32], r5);
    _mm256_store_si256((reg*) &a[64], r2);
    _mm256_store_si256((reg*) &a[96], r6);
    _mm256_store_si256((reg*) &a[128], r3);
    _mm256_store_si256((reg*) &a[160], r7);
    _mm256_store_si256((reg*) &a[192], r4);
    _mm256_store_si256((reg*) &a[224], r8);
    
    const int W = 8;
    for (int i = 0; i < 256 / W; i++) {
        for (int j = 0; j < W; j++)
            printf("%3d ", a[i * W + j]);
        printf("\n");
        if (i % 4 == 3)
            printf("\n");
    }

    return 0;
}
