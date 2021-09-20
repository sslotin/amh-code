#include "gcd.hh"
#include <x86intrin.h>

typedef __m128i reg;

void print(reg x) {
    cout << _mm_cvtsi128_si32(x) << endl;
}

int gcd(int a, int b) {
    if (!a || !b) return a | b;

    char az = __builtin_ctz(a);
    char bz = __builtin_ctz(b);
    char shift = min(az, bz);
    a >>= az;
    b >>= shift;

    reg ar = _mm_set1_epi32(a);
    reg br = _mm_set1_epi32(b);

    // if even:
    //   divide first by 2
    //   (try other powers?)
    // else:
    //   abs, min
    // blend
    
    //_mm_insert_epi32
    //_mm_cvtsi128_si32
    //_mm_abs_epi32
    //_mm_min_epi32
    //_mm_bsrli_si128

    const reg zero = _mm_setzero_si128();
    
    //int cnt = 0;
    while (true) {
    //    cnt++;
    //for (int i = 0; i < 100; i++) {
        // predicate: a is always odd, b may be even
        
        // if b is even
        reg b_even = _mm_srli_epi32(br, 1);

        //cout <<  _mm_cvtsi128_si32(ar) << " " <<  _mm_cvtsi128_si32(br) << " " << _mm_cvtsi128_si32(b_even) << endl;

        // if b is odd
        reg diff = _mm_sub_epi32(ar, br);
        reg a_odd = _mm_min_epi32(ar, br);
        reg b_odd = _mm_abs_epi32(diff);

        reg t = _mm_slli_epi32(br, 31);
        //reg t = br;

        //bool pred = _mm_cvtsi128_si32(ar) != _mm_cvtsi128_si32(a_odd);
        /*
        if (pred) {
            cout << endl;
            print(a_odd);
            print(ar);
            print(t);
            print(br);
        }
        */
        ar = (reg) _mm_blendv_ps((__m128) ar, (__m128) a_odd, (__m128) t);
        //if (pred)
        //    print(ar);
        br = (reg) _mm_blendv_ps((__m128) b_even, (__m128) b_odd, (__m128) t);

        if (_mm_cvtsi128_si32(br) == 0)
            break;
        //reg mask = _mm_cmpeq_epi32(br, zero);
        //int m = _mm_movemask_ps((__m128) mask);
        //if (m != 0)
        //    break;
    }

    //cout << "cnt " << cnt << endl;

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
