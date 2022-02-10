#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i v8i;
typedef __m128i v4i;

v4i broadcast(int *p) {
    return (v4i) _mm_broadcast_ss((float*) p);
}

void prefix(int *p) {
    v8i x = _mm256_load_si256((v8i*) p);
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    _mm256_store_si256((v8i*) p, x);
}

v4i update(int *p, v4i s) {
    v4i d = broadcast(&p[3]);
    v4i x = _mm_load_si128((v4i*) p);
    x = _mm_add_epi32(s, x);
    _mm_store_si128((v4i*) p, x);
    return _mm_add_epi32(s, d);
}

const int B = (1 << 6);

void prefix(int *a, int n) {
    v4i s = _mm_setzero_si128();

    for (int i = 0; i < B; i += 8)
        prefix(&a[i]);

    for (int i = B; i < n; i += 8) {
        prefix(&a[i]);
        s = update(&a[i - B], s);
        s = update(&a[i - B + 4], s);
    }

    for (int i = n - B; i < n; i += 4)
        s = update(&a[i], s);
}
