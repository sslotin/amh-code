#include "argmin.hh"
#include <x86intrin.h>

typedef __m256i reg;

int argmin() {
    int min = INT_MAX, idx = 0;
    
    reg p = _mm256_set1_epi32(min);

    for (int i = 0; i < n; i += 8) {
        reg y = _mm256_load_si256((reg*) &a[i]); 
        reg t = _mm256_min_epi32(t, y);
        reg mask = _mm256_cmpeq_epi32(t, p);
        int m = _mm256_movemask_epi8(mask);
        if (m != -1) { [[unlikely]]
            for (int j = i; j < i + 8; j++)
                if (a[j] < min)
                    min = a[idx = j];
            t = p = _mm256_set1_epi32(min);
        }
    }
    
    return idx;
}
