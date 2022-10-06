#include <bits/stdc++.h>
#include <sys/mman.h>
#include <unistd.h>
#include <x86intrin.h>

typedef __m256i reg;

const int m = 1e5;
const int limit = UINT_MAX / m + 1;

constexpr int crc(int d, int i) {
    return (d + int('0')) * i;
}

struct Precalc {
    // todo: store high and digits together, maybe mask them
    int high[limit];
    int low[6 * m];
    int digits[limit];

    constexpr Precalc() : high{}, low{}, digits{} {
        for (int x = 0; x < limit; x++) {
            for (int y = x; y != 0; y /= 10)
                digits[x]++;
            for (int y = x, d = digits[x] - 1; y != 0; y /= 10, d--)
                high[x] += crc(y % 10, d);
        }
        for (int d = 0; d < 6; d++) {
            for (int x = 0; x < m; x++) {
                if (d == 0) {
                    int t = 0;
                    for (int y = x; y != 0; y /= 10)
                        t++;
                    for (int y = x; y != 0; y /= 10, t--)
                        low[d * m + x] += crc(y % 10, t - 1);
                } else {
                    int t = d + 5;
                    for (int i = 0, y = x; i < 5; i++, t--, y /= 10)
                        low[d * m + x] += crc(y % 10, t - 1);

                }
            }
        }
    }
};

constexpr Precalc P;

int hsum(reg x) {
    int buffer[8], s = 0;
    _mm256_storeu_si256((reg*) buffer, x);
    
    for (int i = 0; i < 8; i++)
        s += buffer[i];

    return s;
}

void print(reg x) {
    int buffer[8];
    _mm256_storeu_si256((reg*) buffer, x);
    for (int i = 0; i < 8; i++)
        std::cout << buffer[i] << " ";
    std::cout << std::endl;
}

int main() {
    int fsize = lseek(0, 0, SEEK_END);
    auto input = (uint32_t*) mmap(0, fsize, PROT_READ, MAP_PRIVATE, 0, 0);

    const int STEP = 1e5;
    const reg mvec = _mm256_set1_epi32(m);
    const reg minv = _mm256_set1_epi32(175921861);

    uint64_t res = 0;

    for (int i = 0; i < fsize / 4; i += STEP) {
        reg s{};
        for (int j = i; j < i + STEP; j += 8) {
            // uint32_t x = input[i];
            reg x = _mm256_load_si256((reg*) &input[j]);
            //print(x);
            // uint32_t hi = x / m;
            // shr  eax, 5
            reg shr = _mm256_srli_epi32(x, 5);
            //print(shr);
            // imul rax, rax, 175921861
            reg mul0 = _mm256_mul_epu32(shr, minv);
            reg mul1 = _mm256_mul_epu32(_mm256_srli_epi64(shr, 32), minv);
            // shr  rax, 39
            reg h0 = _mm256_srli_epi64(mul0, 39);
            reg h1 = _mm256_srli_epi64(mul1, 39);
            //print(h0);
            //print(h1);
            // low 32-bit now contain results, need to merge them
            h1 = _mm256_slli_epi64(h1, 32);
            reg h = _mm256_or_si256(h0, h1);
            //print(h);
            
            // P.high[hi]
            reg hi = _mm256_i32gather_epi32(P.high, h, 4);
            //print(hi);
            // int d = P.digits[hi];
            reg d = _mm256_i32gather_epi32(P.digits, h, 4);
            //print(d);
            // uint32_t lo = x - hi * m;
            reg l = _mm256_sub_epi32(x, _mm256_mullo_epi32(h, mvec));
            //print(l);
            // P.low[d][lo]
            reg idx = _mm256_add_epi32(_mm256_mullo_epi32(d, mvec), l);
            reg lo = _mm256_i32gather_epi32(P.low, idx, 4);
            //print(lo);
            
            // res += ...
            s = _mm256_add_epi32(s, lo);
            s = _mm256_add_epi32(s, hi);
            //print(s);
        }
        res += hsum(s);
    }

    std::cout << res << std::endl;

    return 0;
}
