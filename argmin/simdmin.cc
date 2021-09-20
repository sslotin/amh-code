#include "argmin.hh"
#include <x86intrin.h>

const int B = 8;
typedef int vec __attribute__ (( vector_size(4 * B) ));

void print(vec x) {
    for (int i = 0; i < B; i++)
        std::cout << x[i] << " ";
    std::cout << std::endl;
}

vec min(vec x, vec y) {
    return (x < y ? x : y);
}

int argmin() {
    vec *v = (vec*) a;
    
    int m = INT_MAX, k = 0;
    vec t0, t1, p;
    t0 = t1 = p = m + vec{};

    for (int i = 0; i < n / B; i += 2) {
        t0 = min(t0, v[i]);
        t1 = min(t1, v[i + 1]);
        vec t = min(t0, t1);
        int mask = _mm256_movemask_epi8((__m256i) (p == t));
        //std::cout << std::bitset<32>(mask) << std::endl;
        if (mask != -1) [[unlikely]] {
            /*
            print(t0);
            print(t1);
            print(t);
            print(p);
            */
            //std::cout << i << " " << m << " " << k << std::endl;
            for (int j = B * i; j < B * i + 2 * B; j++)
                if (a[j] < m)
                    m = a[k = j];
            t0 = t1 = p = m + vec{};
        }
    }
    
    return k;
}
