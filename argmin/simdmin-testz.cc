#include "argmin.hh"
#include <x86intrin.h>

const int B = 8;
typedef int vec __attribute__ (( vector_size(4 * B) ));

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

const __m256i ones = _mm256_set1_epi32(-1);

int argmin() {
    vec *v = (vec*) a;
    
    int m = INT_MAX, k = 0;
    vec t0, t1, p;
    t0 = t1 = p = m + vec{};

    for (int i = 0; i < n / B; i += 2) {
        t0 = min(t0, v[i]);
        t1 = min(t1, v[i + 1]);
        vec t = min(t0, t1);
        __m256i mask = _mm256_cmpeq_epi32((__m256i) p, (__m256i) t);
        mask = _mm256_xor_si256(mask, ones);
        //int mask = _mm256_movemask_epi8((__m256i) (p == t));
        if (!_mm256_testz_si256(mask, mask)) { [[unlikely]]
            for (int j = B * i; j < B * i + 2 * B; j++)
                if (a[j] < m)
                    m = a[k = j];
            t0 = t1 = p = m + vec{};
        }
    }
    
    return k;
}
