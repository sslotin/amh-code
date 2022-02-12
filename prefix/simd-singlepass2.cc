#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i v8i;
typedef __m128i v4i;

// _mm256_extracti128_si256
// there is also "extractf" -- what is the difference?

const v8i bcast = _mm256_setr_epi32(3, 3, 3, 3, 7, 7, 7, 7);

void prefix(int *a, int n) {
    v4i s = _mm_setzero_si128();

    for (int i = 0; i < n; i += 8) {
        __builtin_prefetch(&a[i + (1 << 10)]);
        v8i x = _mm256_load_si256((v8i*) &a[i]);

        x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
        x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));

        //v8i d = _mm256_add_epi32(x, bcast); // test
        v8i d = _mm256_permutevar8x32_epi32(x, bcast); // <- this could be faster

        v4i l = _mm256_extracti128_si256(x, 0);
        v4i h = _mm256_extracti128_si256(x, 1);

        v4i ld = _mm256_extracti128_si256(d, 0);
        v4i hd = _mm256_extracti128_si256(d, 1);
        
        l = _mm_add_epi32(l, s);
        _mm_store_si128((v4i*) &a[i], l);

        s = _mm_add_epi32(s, ld);

        h = _mm_add_epi32(h, s);
        _mm_store_si128((v4i*) &a[i + 4], h);

        s = _mm_add_epi32(s, hd);        
    }
}
