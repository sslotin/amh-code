#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i v8i;
typedef __m128i v4i;

v8i broadcast(int *p) {
    return (v8i) _mm256_broadcast_ss((float*) p);
}

/*
void prefix(int *p) {
    v8i x = _mm256_load_si256((v8i*) p);
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    _mm256_store_si256((v8i*) p, x);
    __builtin_prefetch(p + (1<<10));
}
*/

void prefix(int *p) {
    v8i x = _mm256_load_si256((v8i*) p);
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    const v8i perm = _mm256_setr_epi32(0, 0, 0, 0, 3, 3, 3, 3);
    const v8i mask = _mm256_setr_epi32(0, 0, 0, 0, -1, -1, -1, -1);
    v8i y = _mm256_permutevar8x32_epi32(x, perm);
    y = _mm256_and_si256(y, mask);
    x = _mm256_add_epi32(x, y);
    _mm256_store_si256((v8i*) p, x);
    __builtin_prefetch(p + (1<<10));
}

// _mm256_alignr_epi8

/*
void prefix(int *p) {
    v8i x = _mm256_load_si256((v8i*) p);
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    _mm256_store_si256((v8i*) p, x);
    __builtin_prefetch(p + (1<<10));
}
*/

v8i update(int *p, v8i s) {
    v8i d = broadcast(&p[7]);
    v8i x = _mm256_load_si256((v8i*) p);
    x = _mm256_add_epi32(s, x);
    _mm256_store_si256((v8i*) p, x);
    return _mm256_add_epi32(s, d);
}

const int B = (1 << 6);

void prefix(int *a, int n) {
    v8i s = _mm256_setzero_si256();

    for (int i = 0; i < B; i += 8)
        prefix(&a[i]);

    for (int i = B; i < n; i += 8) {
        prefix(&a[i]);
        s = update(&a[i - B], s);
        //s = update(&a[i - B + 4], s);
    }

    for (int i = n - B; i < n; i += 8)
        s = update(&a[i], s);
}
