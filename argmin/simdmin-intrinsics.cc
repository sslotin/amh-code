#include "argmin.hh"
#include <x86intrin.h>

typedef __m256i reg;

int argmin() {
    int min = INT_MAX, idx = 0;
    
    reg p = _mm256_set1_epi32(m);

    for (int i = 0; i < n; i += 8) {
        reg t = _mm256_min_epi32(t, (reg*) &a[i]);
        
        int mask = _mm256_movemask_epi8((__m256i) (p == t));
        if (mask != -1) { [[unlikely]]
            for (int j = B * i; j < B * i + 2 * B; j++)
                if (a[j] < m)
                    m = a[k = j];
            t = p = m + vec{};
        }
    }
    
    return k;
}
