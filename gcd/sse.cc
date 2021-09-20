#include "gcd.hh"
#include <x86intrin.h>

typedef __m128i reg;

int gcd(int a, int b) {
    if (!a || !b) return a | b;

    char az = __builtin_ctz(a);
    char bz = __builtin_ctz(b);
    char shift = min(az, bz);
    a >>= az;

    reg ar = _mm_set1_epi32(a);
    reg br = _mm_set1_epi32(b);
    
    //_mm_insert_epi32
    //_mm_cvtsi128_si32
    //_mm_abs_epi32
    //_mm_min_epi32
    //_mm_bsrli_si128
    
    while (true) {
        br = _mm_sll_epi32(br, _mm_set1_epi32(bz));
        reg diffr = _mm_sub_epi32(ar, br);
        int diff = _mm_cvtsi128_si32(diffr);
        if (diff == 0) break;
        bz = __builtin_ctz(diff);
        br = _mm_min_epi32(ar, br);
        ar = _mm_abs_epi32(diffr);
    }

    /*
    while (true) {
        b >>= bz;
        int dba = b - a;
        int dab = a - b;
        bz = __builtin_ctz(dba);
        if (dba == 0) break;
        a = min(a, b);
        b = max(dba, dab);
    }
    */
    
    a = _mm_cvtsi128_si32(ar);

    return a << shift;
}
