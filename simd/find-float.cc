#include "find.hh"

typedef __m256 reg;

int find(int needle) {
    float *f = (float*) a;
    reg x = (reg) _mm256_set1_epi32(needle);

    for (int i = 0; i < N; i += 16) {
        reg y1 = _mm256_load_ps(&f[i]);
        reg y2 = _mm256_load_ps(&f[i + 8]);
        reg m1 = _mm256_cmp_ps(x, y1, _CMP_EQ_OQ);
        reg m2 = _mm256_cmp_ps(x, y2, _CMP_EQ_OQ);
        reg m = _mm256_or_ps(m1, m2);
        if (!_mm256_testz_ps(m, m)) {
            int mask = (_mm256_movemask_ps(m2) << 8)
                     +  _mm256_movemask_ps(m1);
            return i + __builtin_ctz(mask);
        }
    }

    return -1;
}
