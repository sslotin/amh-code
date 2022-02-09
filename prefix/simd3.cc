#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i reg;
typedef __m128i res;

reg prefix(reg x) {
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    return x;
}

res bcast(int *p) {
    return (res) _mm_broadcast_ss((float*) p);
}

void prefix(int *a, int *b, int n) {
    for (int i = 0; i < n; i += 8) {
        reg x = _mm256_load_si256((reg*) &a[i]);
        reg p = prefix(x);
        _mm256_store_si256((reg*) &b[i], p);
    }
    
    res s = bcast(&b[3]);
    
    for (int i = 4; i < n; i += 4) {
        res d = bcast(&b[i + 3]);
        res x = _mm_load_si128((res*) &b[i]);
        x = _mm_add_epi32(s, x);
        _mm_store_si128((res*) &b[i], x);
        s = _mm_add_epi32(s, d);
    }
}
