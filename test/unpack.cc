#include <bits/stdc++.h>
#include <x86intrin.h>

typedef __m256i reg;

void print(reg v) {
    auto t = (int*) &v;
    for (int i = 0; i < 8; i++)
        printf("%2d ", t[i]);
    printf("\n");
}

/*
__m256d y = _mm256_permute2f128_pd(x, x, 1); // permute 128-bit values
__m256d m1 = _mm256_max_pd(x, y); // m1[0] = max(x[0], x[2]), m1[1] = max(x[1], x[3]), etc.
__m256d m2 = _mm256_permute_pd(m1, 5); // set m2[0] = m1[1], m2[1] = m1[0], etc.
__m256d m = _mm256_max_pd(m1, m2); // all m[0] ... m[3] contain the horizontal max(x[0], x[1], x[2], x[3])
*/

int main() {
    reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
    reg y = _mm256_setr_epi32(-1, -2, -3, -4, -5, -6, -7, -8);
    reg l = _mm256_unpacklo_epi32(x, y);
    reg h = _mm256_unpackhi_epi32(x, y);

    print(l);
    print(h);

    //print(_mm256_permute2f128_si256(x, y, 0));
    print(_mm256_permute2x128_si256(x, y, 1)); // swap left and right
    print((reg) _mm256_permute_ps( (__m256) x, 1 + (0 << 2) + (3 << 4) + (2 << 6))); // swap odd-even
    print((reg) _mm256_permute_pd( (__m256d) x, 5));
    
    /*
    print(_mm256_permute2f128_si256(x, y, 2));
    print(_mm256_permute2f128_si256(x, y, 3));
    print(_mm256_permute2f128_si256(x, y, 4));
    print(_mm256_permute2f128_si256(x, y, 4));
    print(_mm256_permute2f128_si256(x, y, 4));
    print(_mm256_permute2f128_si256(x, y, 4));
    print(_mm256_permute2f128_si256(x, y, 4));
    */
    
    return 0;
}