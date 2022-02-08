#include "argmin.hh"
#include <x86intrin.h>

typedef __m256 reg;

int argmin(int *ai, int n) {
    float *a = (float*) ai;
    float min = std::numeric_limits<float>::infinity();
    int idx = 0;
    
    reg p = _mm256_set1_ps(min);

    for (int i = 0; i < n; i += 32) {
        reg y1 = _mm256_load_ps(&a[i]);
        reg y2 = _mm256_load_ps(&a[i + 8]);
        reg y3 = _mm256_load_ps(&a[i + 16]);
        reg y4 = _mm256_load_ps(&a[i + 24]);
        y1 = _mm256_min_ps(y1, y2);
        y3 = _mm256_min_ps(y3, y4);
        y1 = _mm256_min_ps(y1, y3);
        reg mask = _mm256_cmp_ps(p, y1, _CMP_GE_OQ);
        if (!_mm256_testz_ps(mask, mask)) { [[unlikely]]
            for (int j = i; j < i + 32; j++)
                if (a[j] < min)
                    min = a[idx = j];
            p = _mm256_set1_ps(min);
        }
    }
    
    return idx;
}
