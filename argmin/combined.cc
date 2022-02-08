#include "argmin.hh"
#include <x86intrin.h>

const int B = 8, T = 2;
typedef int vec __attribute__ (( vector_size(4 * B) ));


typedef __m256i reg;

unsigned get_mask(reg m) {
    return _mm256_movemask_ps((__m256) m);
}

reg cmp(reg x, int *p) {
    reg y = _mm256_load_si256( (reg*) p );
    return _mm256_cmpeq_epi32(x, y);
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

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

int argmin(int *a, int n) {
    vec *v = (vec*) a;
    
    vec t[K];
    for (int i = 0; i < T; i++)
        t[i] = INT_MAX + vec{};

    for (int i = 0; i < n / B; i += T)
        for (int j = 0; j < T; j++)
            t[j] = min(t[j], v[i + j]);
    
    for (int i = 1; i < T; i++)
        t[0] = min(t[0], t[i]);

    int m = t[0][0];

    for (int i = 1; i < B; i++)
        m = std::min(m, t[0][i]);

    return m;
}

const int B = 256;

int argmin(int *a, int n) {
    for ()
}
