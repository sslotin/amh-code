#include <bits/stdc++.h>
#include <x86intrin.h>

typedef __m256i reg;

alignas(64) int a[64], b[64];

int main() {
    for (int i = 0; i < 64; i++)
        a[i] = i;

    reg r1 = _mm256_load_si256((reg*) &a[0]);
    reg r3 = _mm256_load_si256((reg*) &a[8]);
    reg r2 = _mm256_load_si256((reg*) &a[16]);
    reg r4 = _mm256_load_si256((reg*) &a[24]);
    reg r5 = _mm256_load_si256((reg*) &a[32]);
    reg r7 = _mm256_load_si256((reg*) &a[40]);
    reg r6 = _mm256_load_si256((reg*) &a[48]);
    reg r8 = _mm256_load_si256((reg*) &a[56]);

    reg d1 = _mm256_unpacklo_epi32(r1, r2);
    reg d2 = _mm256_unpackhi_epi32(r1, r2);
    reg d3 = _mm256_unpacklo_epi32(r3, r4);
    reg d4 = _mm256_unpackhi_epi32(r3, r4);
    reg d5 = _mm256_unpacklo_epi32(r5, r6);
    reg d6 = _mm256_unpackhi_epi32(r5, r6);
    reg d7 = _mm256_unpacklo_epi32(r7, r8);
    reg d8 = _mm256_unpackhi_epi32(r7, r8);

    reg q1 = _mm256_unpacklo_epi32(d1, d3);
    reg q2 = _mm256_unpackhi_epi32(d1, d3);
    reg q3 = _mm256_unpacklo_epi32(d2, d4);
    reg q4 = _mm256_unpackhi_epi32(d2, d4);
    reg q5 = _mm256_unpacklo_epi32(d5, d7);
    reg q6 = _mm256_unpackhi_epi32(d5, d7);
    reg q7 = _mm256_unpacklo_epi32(d6, d8);
    reg q8 = _mm256_unpackhi_epi32(d6, d8);

    /*
    r1 = q1;
    r2 = q2;
    r3 = q3;
    r4 = q4;
    r5 = q5;
    r6 = q6;
    r7 = q7;
    r8 = q8;
    */

    r1 = _mm256_permute2f128_si256(q1, q5, 0x20);
    r2 = _mm256_permute2f128_si256(q2, q6, 0x20);
    r3 = _mm256_permute2f128_si256(q3, q7, 0x20);
    r4 = _mm256_permute2f128_si256(q4, q8, 0x20);
    r5 = _mm256_permute2f128_si256(q1, q5, 0x31);
    r6 = _mm256_permute2f128_si256(q2, q6, 0x31);
    r7 = _mm256_permute2f128_si256(q3, q7, 0x31);
    r8 = _mm256_permute2f128_si256(q4, q8, 0x31);

    /* 
        row0 = _mm256_permute2f128_ps(__tt0, __tt4, 0x20);
        row1 = _mm256_permute2f128_ps(__tt1, __tt5, 0x20);
        row2 = _mm256_permute2f128_ps(__tt2, __tt6, 0x20);
        row3 = _mm256_permute2f128_ps(__tt3, __tt7, 0x20);
        row4 = _mm256_permute2f128_ps(__tt0, __tt4, 0x31);
        row5 = _mm256_permute2f128_ps(__tt1, __tt5, 0x31);
        row6 = _mm256_permute2f128_ps(__tt2, __tt6, 0x31);
        row7 = _mm256_permute2f128_ps(__tt3, __tt7, 0x31);
    */
    
    _mm256_store_si256((reg*) &a[0], r1);
    _mm256_store_si256((reg*) &a[8], r2);
    _mm256_store_si256((reg*) &a[16], r3);
    _mm256_store_si256((reg*) &a[24], r4);
    _mm256_store_si256((reg*) &a[32], r5);
    _mm256_store_si256((reg*) &a[40], r6);
    _mm256_store_si256((reg*) &a[48], r7);
    _mm256_store_si256((reg*) &a[56], r8);
    
    const int W = 8;
    for (int i = 0; i < 64 / W; i++) {
        for (int j = 0; j < W; j++)
            printf("%3d ", a[i * W + j]);
        printf("\n");
    }

    return 0;
}
