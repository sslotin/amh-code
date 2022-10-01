#include "read.hh"
#include <sys/mman.h>

typedef __m256i reg;

void transpose(char *a, reg *b, int P) {
    reg r1 = _mm256_load_si256((reg*) &a[0 * P]);
    reg r5 = _mm256_load_si256((reg*) &a[1 * P]);
    reg r3 = _mm256_load_si256((reg*) &a[2 * P]);
    reg r7 = _mm256_load_si256((reg*) &a[3 * P]);
    reg r2 = _mm256_load_si256((reg*) &a[4 * P]);
    reg r6 = _mm256_load_si256((reg*) &a[5 * P]);
    reg r4 = _mm256_load_si256((reg*) &a[6 * P]);
    reg r8 = _mm256_load_si256((reg*) &a[7 * P]);

    reg d1 = _mm256_unpacklo_epi8(r1, r2);
    reg d2 = _mm256_unpackhi_epi8(r1, r2);
    reg d3 = _mm256_unpacklo_epi8(r3, r4);
    reg d4 = _mm256_unpackhi_epi8(r3, r4);
    reg d5 = _mm256_unpacklo_epi8(r5, r6);
    reg d6 = _mm256_unpackhi_epi8(r5, r6);
    reg d7 = _mm256_unpacklo_epi8(r7, r8);
    reg d8 = _mm256_unpackhi_epi8(r7, r8);

    reg q1 = _mm256_unpacklo_epi8(d1, d3);
    reg q2 = _mm256_unpackhi_epi8(d1, d3);
    reg q3 = _mm256_unpacklo_epi8(d2, d4);
    reg q4 = _mm256_unpackhi_epi8(d2, d4);
    reg q5 = _mm256_unpacklo_epi8(d5, d7);
    reg q6 = _mm256_unpackhi_epi8(d5, d7);
    reg q7 = _mm256_unpacklo_epi8(d6, d8);
    reg q8 = _mm256_unpackhi_epi8(d6, d8);

    r1 = _mm256_unpacklo_epi8(q1, q5);
    r2 = _mm256_unpackhi_epi8(q1, q5);
    r3 = _mm256_unpacklo_epi8(q2, q6);
    r4 = _mm256_unpackhi_epi8(q2, q6);
    r5 = _mm256_unpacklo_epi8(q3, q7);
    r6 = _mm256_unpackhi_epi8(q3, q7);
    r7 = _mm256_unpacklo_epi8(q4, q8);
    r8 = _mm256_unpackhi_epi8(q4, q8);
    
    _mm256_store_si256(b + 0, r1);
    _mm256_store_si256(b + 1, r2);
    _mm256_store_si256(b + 2, r3);
    _mm256_store_si256(b + 3, r4);
    _mm256_store_si256(b + 4, r5);
    _mm256_store_si256(b + 5, r6);
    _mm256_store_si256(b + 6, r7);
    _mm256_store_si256(b + 7, r8);
}

/*
  0  32  64  96 128 160 192 224 
  1  33  65  97 129 161 193 225 
 16  48  80 112 144 176 208 240 
 17  49  81 113 145 177 209 241 

  2  34  66  98 130 162 194 226 
  3  35  67  99 131 163 195 227 
 18  50  82 114 146 178 210 242 
 19  51  83 115 147 179 211 243 
 ...
*/

void print(reg x) {
    int buf[8];
    _mm256_storeu_si256((reg*) buf, x);
    for (int i = 0; i < 8; i++)
        printf("%d ", buf[i]);
    printf("\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

// 256-byte buffer
void update(char *buffer, reg &x, reg &s) {
    for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 8; k++) {
            for (int l = 0; l < 2; l++) {
                const reg zero_char = _mm256_set1_epi32('0');
                const reg zero_int = _mm256_setzero_si256();
                const reg ten = _mm256_set1_epi32(10);

                reg c = _mm256_cvtepu8_epi32(
                    _mm_loadl_epi64((__m128i*) &buffer[j * 16 + k * 32 + l * 8])
                );

                reg mask = _mm256_cmpgt_epi32(zero_char, c); // 1s are separators
                
                c = _mm256_sub_epi32(c, zero_char);

                reg y = _mm256_add_epi32(_mm256_mullo_epi32(x, ten), c);
                s = _mm256_xor_si256(s, _mm256_and_si256(x, mask));

                x = _mm256_blendv_epi8(y, zero_int, mask);
                //print(c);
                //print(x);
                //print(s);
                //std::cout << std::endl;
            }
        }
    }
}

int hsum(reg x) {
    int buffer[8], s = 0;
    _mm256_storeu_si256((reg*) buffer, x);
    
    for (int i = 0; i < 8; i++)
        s ^= buffer[i];

    return s;
}

int parse_int(char *s) {
    int x = 0;

    while (*s != '\n') {
        x = 10 * x + (*s - '0');
        s++;
    }

    return x;
}

int read(int n) {
    int fsize = lseek(0, 0, SEEK_END);
    auto input = (char*) mmap(0, fsize, PROT_READ, MAP_PRIVATE, 0, 0);
    // pretty sure in all sane OSes stdin starts on a memory page (and hence cache line)

    // number of concurrent "threads"
    const int K = 2;

    // there should be 8 partitions
    // partition size should be divisible by 32
    // also there are probably some minimum restrictions
    int P = fsize / K / 256 * 32;

    reg s[K] = {}, x[K] = {};

    for (int i = 0; i < P; i += 32) {
        for (int j = 0; j < K; j++) {
            alignas(32) char buffer[8 * 32];
            transpose(input + 8 * P * j + i, (reg*) buffer, P);
            update(buffer, x[j], s[j]);
        }
    }

    for (int j = 1; j < K; j++)
        s[0] = _mm256_xor_si256(s[0], s[j]);

    int r = hsum(s[0]);

    // merge partitions
    for (int i = 1; i < 8 * K; i++) {
        char *k = input + i * P - 1; // last char of the previous partition
        if (*k != '\n') {
            r ^= parse_int(k + 1);
            //std::cout << i << " " << parse_int(k + 1) << std::endl;
            while (*k != '\n')
                k--;           
            r ^= parse_int(k + 1);
            //std::cout << i << " " << parse_int(k + 1) << std::endl;
        }
    }

    // parse the rest
    int k = 8 * K * P, y = 0;
    while (input[k - 1] != '\n')
        k--;

    while (k < fsize) {
        char c = input[k++];
        if (c != '\n')
            y = 10 * y + (c - '0');
        else {
            //std::cout << y << std::endl;
            r ^= y;
            y = 0;
        }
    }

    return r;
}
