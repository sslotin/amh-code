#include "argmin.hh"
#include <x86intrin.h>

/*
int argmin(int *a, int n) {
    int needle = min(a, n);
    int idx = find(a, n, needle);
    return idx;
}
*/

typedef __m256i reg;

unsigned get_mask(reg m) {
    return _mm256_movemask_ps((__m256) m);
}

reg cmp(reg x, int *p) {
    reg y = _mm256_load_si256( (reg*) p );
    return _mm256_cmpeq_epi32(x, y);
}

reg min(reg x, int *p) {
    reg y = _mm256_load_si256( (reg*) p );
    return _mm256_min_epi32(x, y);
}

int find(int *a, int n, int needle) {
    reg x = _mm256_set1_epi32(needle);

    for (int i = 0; i < n; i += 32) {
        reg m1 = cmp(x, &a[i]);
        reg m2 = cmp(x, &a[i + 8]);
        reg m3 = cmp(x, &a[i + 16]);
        reg m4 = cmp(x, &a[i + 24]);
        reg m12 = _mm256_or_si256(m1, m2);
        reg m34 = _mm256_or_si256(m3, m4);
        reg m = _mm256_or_si256(m12, m34);
        if (!_mm256_testz_si256(m, m)) {
            unsigned mask = (get_mask(m4) << 24)
                          + (get_mask(m3) << 16)
                          + (get_mask(m2) << 8)
                          +  get_mask(m1);
            return i + __builtin_ctz(mask);
        }
    }

    return -1;
}

const int B = 2048;

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
    reg m, m1, m2;
    m = m1 = m2 = _mm256_set1_epi32(INT_MAX);

    for (int i = 0; i < N; i += B) {
        for (int j = i; j < i + B; j += 16) {
            m1 = min(m1, &a[j]);
            m2 = min(m2, &a[j + 8]);
        }
        reg t = _mm256_min_epi32(m1, m2);
        reg mask = _mm256_cmpgt_epi32(m, t);
        if (!_mm256_testz_si256(mask, mask)) {
            idx = i;
            m = hmin(t);
            //printf("%d %d\n", idx, _mm256_extract_epi32(m, 0));
        }
    }

    //printf("%d %d\n", idx, _mm256_extract_epi32(m, 0));

    return idx + find(a + idx, B, _mm256_extract_epi32(m, 0));
}
