#include "argmin.hh"
#include <x86intrin.h>

typedef __m256i reg;

reg hmin(reg x) {
    // 2  1  4  3  6  5  8  7 
    reg y = (reg) _mm256_permute_ps( (__m256) x, 1 + (0 << 2) + (3 << 4) + (2 << 6));
    x = _mm256_min_epi32(x, y);
    // 2  1  4  3  6  5  8  7 
    y = (reg) _mm256_permute_pd( (__m256d) x, 5);
    x = _mm256_min_epi32(x, y);
    // 5  6  7  8  1  2  3  4 
    y = _mm256_permute2x128_si256(x, y, 1);
    x = _mm256_min_epi32(x, y);
    return x;
}

int argmin(int *a, int n) {
    int idx = 0;
    
    reg p = _mm256_set1_epi32(INT_MAX);

    for (int i = 0; i < n; i += 32) {
        reg y1 = _mm256_load_si256((reg*) &a[i]);
        reg y2 = _mm256_load_si256((reg*) &a[i + 8]);
        reg y3 = _mm256_load_si256((reg*) &a[i + 16]);
        reg y4 = _mm256_load_si256((reg*) &a[i + 24]);
        y1 = _mm256_min_epi32(y1, y2);
        y3 = _mm256_min_epi32(y3, y4);
        y1 = _mm256_min_epi32(y1, y3);
        reg mask = _mm256_cmpgt_epi32(p, y1);
        if (!_mm256_testz_si256(mask, mask)) { [[unlikely]]
            idx = i;
            p = hmin(y1);
        }
    }

    int min = _mm256_extract_epi32(p, 0);

    for (int i = idx; i < idx + 31; i++)
        if (a[i] == min)
            return i;
    
    return idx + 31;
}
