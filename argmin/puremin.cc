#include "argmin.hh"
#include <x86intrin.h>

const int B = 8, T = 4;
typedef int vec __attribute__ (( vector_size(4 * B) ));

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

int argmin(int *a, int n) {
    vec *v = (vec*) a;
    
    vec t[T];
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
