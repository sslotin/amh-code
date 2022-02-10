#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i v8i;
typedef __m128i v4i;

v4i broadcast(int *p) {
    return (v4i) _mm_broadcast_ss((float*) p);
}

const int B = (1 << 12);

v4i local_prefix(int *a, v4i s) {
    for (int i = 0; i < B; i += 8) {
        v8i x = _mm256_load_si256((v8i*) &a[i]);
        x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
        x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
        _mm256_store_si256((v8i*) &a[i], x);
    }

    for (int i = 0; i < B; i += 4) {
        __builtin_prefetch(&a[i + B]);
        v4i d = broadcast(&a[i + 3]);
        v4i x = _mm_load_si128((v4i*) &a[i]);
        x = _mm_add_epi32(s, x);
        _mm_store_si128((v4i*) &a[i], x);
        s = _mm_add_epi32(s, d);
    }

    return s;
}

void prefix(int *a, int n) {
    v4i s = _mm_setzero_si128();
    for (int i = 0; i < n; i += B)
        s = local_prefix(a + i, s);
}
