#include "argmin.hh"
#include <x86intrin.h>

typedef __m256i reg;

int argmin(int *a, int n) {
    const reg eight = _mm256_set1_epi32(8);

    reg cur = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    reg min = _mm256_set1_epi32(INT_MAX);
    reg idx = _mm256_setzero_si256();

    for (int i = 0; i < n; i += 8) {
        reg x = _mm256_load_si256((reg*) &a[i]);
        reg mask = _mm256_cmpgt_epi32(min, x);
        idx = _mm256_blendv_epi8(idx, cur, mask);
        //min = _mm256_blendv_epi8(min, x, mask);
        min = _mm256_min_epi32(x, min);
        cur = _mm256_add_epi32(cur, eight);
    }

    int min_arr[8], idx_arr[8];
    
    _mm256_storeu_si256((reg*) min_arr, min);
    _mm256_storeu_si256((reg*) idx_arr, idx);

    /*
    for (int i = 0; i < 8; i++)
        printf("%d ", min_arr[i]);
    printf("\n");

    for (int i = 0; i < 8; i++)
        printf("%d ", idx_arr[i]);
    printf("\n");
    */

    int k = 0, m = min_arr[0];

    for (int i = 1; i < 8; i++)
        if (min_arr[i] < m)
            m = min_arr[k = i];
    
    //printf("%d\n", k);
    //printf("%d\n", a[35545]);

    return idx_arr[k];
}
