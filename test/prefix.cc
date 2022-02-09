#include <bits/stdc++.h>
#include <x86intrin.h>

typedef __m256i reg;

void print(reg v) {
    auto t = (int*) &v;
    for (int i = 0; i < 8; i++)
        printf("%2d ", t[i]);
    printf("\n");
}

reg prefix(reg x) {
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 16));
    return x;
}

int main() {
    reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
    print(prefix(x));

    /*
    print(_mm256_permute2x128_si256(x, x, 0));
    print(_mm256_permute2x128_si256(x, x, 1));
    print(_mm256_permute2x128_si256(x, x, 2));
    print(_mm256_permute2x128_si256(x, x, 3));
    print(_mm256_permute2x128_si256(x, x, 4));
    print(_mm256_permute2x128_si256(x, x, 5));
    print(_mm256_permute2x128_si256(x, x, 6));
    print(_mm256_permute2x128_si256(x, x, 7));
    print(_mm256_permute2x128_si256(x, x, 8));
    print(_mm256_permute2x128_si256(x, x, 9));
    print(_mm256_permute2x128_si256(x, x, 10));
    print(_mm256_permute2x128_si256(x, x, 11));
    print(_mm256_permute2x128_si256(x, x, 12));
    print(_mm256_permute2x128_si256(x, x, 13));
    */
    print(_mm256_permute2x128_si256(x, x, 8));

    return 0;
}
