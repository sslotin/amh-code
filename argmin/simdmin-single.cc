#include "argmin.hh"
#include <x86intrin.h>

const int B = 8;
typedef int vec __attribute__ (( vector_size(4 * B) ));

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

int mask(vec x) {
    return _mm256_movemask_epi8((__m256i) x);
}

bool zero(vec x) {
    return _mm256_testz_si256((__m256i) x, (__m256i) x);
}

int argmin() {
    vec *v = (vec*) a;
    
    int ms = INT_MAX, idx = 0;
    vec mv = ms + vec{};

    for (int i = 0; i < n / B; i++) {
        vec t = min(t, v[i]);
        if (mask(p == t) != -1) { [[unlikely]]
            for (int j = 0; j < 8; j++)
                if (a[8 * i + j] < m)
                    m = a[k = 8 * i + j];
            t = mv = m + vec{};
        }
    }
    
    return k;
}
