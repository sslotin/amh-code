#include "argmin.hh"
#include <x86intrin.h>

const int B = 8;
typedef int vec __attribute__ (( vector_size(4 * B) ));

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

int argmin() {
    vec *v = (vec*) a;
    
    int m = INT_MAX, k = 0;
    vec p, t;
    t = p = m + vec{};

    for (int i = 0; i < n / B; i++) {
        t = min(t, v[i]);
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
