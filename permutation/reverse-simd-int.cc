#include "permutation.hh"

typedef __m256i reg;

void prepare(int n) {}

const reg reverseMask = _mm256_setr_epi32(
    7, 6, 5, 4, 3, 2, 1, 0
);

reg reverse(reg x) {
    x = _mm256_permutevar8x32_epi32(x, reverseMask);
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
