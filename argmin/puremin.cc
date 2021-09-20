#include "argmin.hh"
#include <x86intrin.h>

const int B = 8, K = 2;
typedef int vec __attribute__ (( vector_size(4 * B) ));

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

int argmin() {
    vec *v = (vec*) a;
    
    vec t[K];
    for (int i = 0; i < K; i++)
        t[i] = INT_MAX + vec{};

    for (int i = 0; i < n / B; i += K)
        for (int j = 0; j < K; j++)
            t[j] = min(t[j], v[i + j]);
    
    for (int i = 1; i < K; i++)
        t[0] = min(t[0], t[i]);

    int m = t[0][0];

    for (int i = 1; i < B; i++)
        m = std::min(m, t[0][i]);

    return m;
}
