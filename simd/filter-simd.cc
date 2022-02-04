#include "filter.hh"

typedef __m256i reg;

struct Precalc {
    alignas(64) int permutation[256][8];

    constexpr Precalc() : permutation{} {
        for (int m = 0; m < 256; m++) {
            int k = 0;
            for (int i = 0; i < 8; i++)
                if (m >> i & 1)
                    permutation[m][k++] = i;
            assert(__builtin_popcount(m) == k);
            /*
                if (m >> i & 1) {
                    permutation[m][l++] = i;
                } else {
                    permutation[m][r--] = i;
                }
            */
        }
    }
};

constexpr Precalc T;

const reg p = _mm256_set1_epi32(P);

int filter() {
    int k = 0;

    for (int i = 0; i < N; i += 8) {
        reg x = _mm256_load_si256( (reg*) &a[i] );
        
        reg m = _mm256_cmpgt_epi32(p, x);
        int mask = _mm256_movemask_ps((__m256) m);
        reg permutation = _mm256_load_si256( (reg*) &T.permutation[mask] );
        
        x = _mm256_permutevar8x32_epi32(x, permutation);
        _mm256_storeu_si256((reg*) &b[k], x);
        
        k += __builtin_popcount(mask);
    }

    /*
    for (int i = 0; i < N; i++)
        std::cerr << a[i] << " ";

    std::cerr << std::endl;

    for (int i = 0; i < N; i++)
        std::cerr << b[i] << " ";
    
    std::cerr << std::endl;
    */

    return k;
}
