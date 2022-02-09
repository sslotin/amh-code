#include "prefix.hh"
#include <x86intrin.h>

typedef __m256i reg;
typedef __m128i res;

reg prefix(reg x) {
    //const reg half = _mm256_setr_epi32(0, 0, 0, 0, -1, -1, -1, -1);
    //const reg perm = _mm256_setr_epi32(0, 0, 0, 0, 3, 3, 3, 3);
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 4));
    x = _mm256_add_epi32(x, _mm256_slli_si256(x, 8));
    //reg y = _mm256_permutevar8x32_epi32(x, perm);
    //y = _mm256_and_si256(y, half);
    //x = _mm256_add_epi32(x, y);
    //x = _mm256_add_epi32(x, _mm256_slli_si256(x, 16));
    //x = _mm256_add_epi32(x, _mm256_permute2x128_si256(x, x, 8));
    // _mm256_permutevar8x32_epi32
    return x;
}

res bcast(int *p) {
    return (res) _mm_broadcast_ss((float*) p);
}

void prefix(int *a, int *b, int n) {
    //reg s = _mm256_setzero_si256();
    for (int i = 0; i < n; i += 8) {
        reg x = _mm256_load_si256((reg*) &a[i]);
        reg p = prefix(x);
        //p = _mm256_add_epi32(p, s);
        _mm256_store_si256((reg*) &b[i], p);
        //s = _mm256_set1_epi32(b[i + 7]);
    }
    
    
    res s = bcast(&b[3]);
    
    for (int i = 4; i < n; i += 4) {
        // add v, m
        // broardcast v, m
        // store m, v
        // add v, v
        
        //res x = _mm_load_si128((res*) &b[i]);
        //res y = _mm_load_si128((res*) &b[i + 4]);
        //res z = _mm_add_epi32(x, y);
        //_mm_store_si128((res*) &b[i], z);

        res d = bcast(&b[i + 3]);
        res x = _mm_load_si128((res*) &b[i]);
        x = _mm_add_epi32(s, x);
        _mm_store_si128((res*) &b[i], x);
        s = _mm_add_epi32(s, d);
    }
    
}
