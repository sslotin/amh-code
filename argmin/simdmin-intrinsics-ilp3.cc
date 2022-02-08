#include "argmin.hh"
#include <x86intrin.h>

typedef __m256i reg;

int argmin(int *a, int n) {
    int min = INT_MAX, idx = 0;
    
    reg p = _mm256_set1_epi32(min);

    //int cnt = 0;

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
            for (int j = i; j < i + 32; j++)
                min = (a[j] < min ? a[j] : min);
            p = _mm256_set1_epi32(min);
            //cnt++;
        }
    }

    for (int i = idx; i < idx + 31; i++)
        if (a[i] == min)
            return i;
    
    return idx + 31;
}
