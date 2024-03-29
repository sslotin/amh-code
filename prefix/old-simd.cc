#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i reg;
typedef __m128i res;

reg prefix(reg x) {
    //const reg zeros = _mm256_setzero_si256();
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    //x = _mm256_add_epi32(x, _mm256_slli_si256(x, 16));
    //x = _mm256_add_epi32(x, _mm256_permute2x128_si256(x, x, 8));
    return x;
}

void prefix(int *a, int *b, int n) {
    //reg s = _mm256_setzero_si256();
    for (int i = 0; i < n; i += 8) {
        reg x = _mm256_load_si256((reg*) &a[i]);
        reg p = prefix(x);
        //p = _mm256_add_epi32(p, s);
        _mm256_store_si256((reg*) &b[i], p);
        //s = _mm256_set1_epi32(b[i + 7]);
    }
    res s = _mm_set1_epi32(b[3]);
    for (int i = 4; i < n; i += 4) {
        res x = _mm_load_si128((res*) &b[i]);
        res d = _mm_set1_epi32(b[i + 3]);
        x = _mm_add_epi32(s, x);
        _mm_store_si128((res*) &b[i], x);
        s = _mm_add_epi32(s, d);
    }
}
