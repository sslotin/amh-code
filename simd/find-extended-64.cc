#include "find.hh"

typedef __m256i reg;

unsigned long long get_mask(reg m) {
    return _mm256_movemask_ps((__m256) m);
}

reg cmp(reg x, int *p) {
    reg y = _mm256_load_si256( (reg*) p );
    return _mm256_cmpeq_epi32(x, y);
}

int find(int needle) {
    reg x = _mm256_set1_epi32(needle);

    for (int i = 0; i < N; i += 64) {
        reg m1 = cmp(x, &a[i]);
        reg m2 = cmp(x, &a[i + 8]);
        reg m3 = cmp(x, &a[i + 16]);
        reg m4 = cmp(x, &a[i + 24]);
        reg m5 = cmp(x, &a[i + 32]);
        reg m6 = cmp(x, &a[i + 40]);
        reg m7 = cmp(x, &a[i + 48]);
        reg m8 = cmp(x, &a[i + 56]);
        reg m12 = _mm256_or_si256(m1, m2);
        reg m34 = _mm256_or_si256(m3, m4);
        reg m56 = _mm256_or_si256(m5, m6);
        reg m78 = _mm256_or_si256(m7, m8);
        reg m1234 = _mm256_or_si256(m12, m34);
        reg m5678 = _mm256_or_si256(m56, m78);
        reg m = _mm256_or_si256(m1234, m5678);
        if (!_mm256_testz_si256(m, m)) {
            unsigned long long mask = (get_mask(m8) << 56)
                                    + (get_mask(m7) << 48)
                                    + (get_mask(m6) << 40)
                                    + (get_mask(m5) << 32)
                                    + (get_mask(m4) << 24)
                                    + (get_mask(m3) << 16)
                                    + (get_mask(m2) << 8)
                                    +  get_mask(m1);
            return i + __builtin_ctzll(mask);
        }
    }

    return -1;
}
