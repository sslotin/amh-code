#include "permutation.hh"

typedef __m256i reg;

// _mm256_shuffle_epi8
// _mm256_permute2x128_si256

const reg reverseMask = _mm256_setr_epi8(
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
);

reg reverse(reg x) {
    x = _mm256_shuffle_epi8(x, reverseMask);
    x = _mm256_permute2x128_si256(x, x, 1);
    return x;
}

void permute(char *a, int n) {
    for (int i = 0; i < n / 2; i += 32) {
        reg x = _mm256_loadu_si256((reg*) &a[i]);
        reg y = _mm256_loadu_si256((reg*) &a[n - 32 - i]);

        x = reverse(x);
        y = reverse(y);        

        _mm256_storeu_si256((reg*) &a[n - 32 - i], x);
        _mm256_storeu_si256((reg*) &a[i], y);
    }
}
