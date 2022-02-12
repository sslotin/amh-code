#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i v8i;

// _mm256_extractf128_ps

const v8i perm = _mm256_setr_epi32(0, 0, 0, 0, 3, 3, 3, 3);
const v8i mask = _mm256_setr_epi32(0, 0, 0, 0, -1, -1, -1, -1);
const v8i bcast = _mm256_setr_epi32(7, 7, 7, 7, 7, 7, 7, 7);

void prefix(int *a, int n) {
    v8i s = _mm256_setzero_si256();

    for (int i = 0; i < n; i += 8) {
        __builtin_prefetch(&a[i + (1 << 10)]);
        v8i x = _mm256_load_si256((v8i*) &a[i]);

        x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
        x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));

        v8i y = _mm256_permutevar8x32_epi32(x, perm);
        y = _mm256_and_si256(y, mask);
        x = _mm256_add_epi32(x, y);

        v8i d = _mm256_permutevar8x32_epi32(x, bcast);
        
        x = _mm256_add_epi32(x, s);
        s = _mm256_add_epi32(s, d);
        
        _mm256_store_si256((v8i*) &a[i], x);   
    }
}
