#include "read.hh"

void prepare() {}

const int SIZE = (1<<16); // 16K
const int K = 4; 
const int P = SIZE / K / 8;   // partition size

typedef __m256i reg;

void transpose(char *a, char *b) {
    for (int i = 0; i < P; i += 32) {
        reg r1 = _mm256_load_si256((reg*) &a[0 * P + i]);
        reg r5 = _mm256_load_si256((reg*) &a[1 * P + i]);
        reg r3 = _mm256_load_si256((reg*) &a[2 * P + i]);
        reg r7 = _mm256_load_si256((reg*) &a[3 * P + i]);
        reg r2 = _mm256_load_si256((reg*) &a[4 * P + i]);
        reg r6 = _mm256_load_si256((reg*) &a[5 * P + i]);
        reg r4 = _mm256_load_si256((reg*) &a[6 * P + i]);
        reg r8 = _mm256_load_si256((reg*) &a[7 * P + i]);

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
        
        _mm256_store_si256((reg*) &b[8 * i + 0], r1);
        _mm256_store_si256((reg*) &b[8 * i + 32], r2);
        _mm256_store_si256((reg*) &b[8 * i + 64], r3);
        _mm256_store_si256((reg*) &b[8 * i + 96], r4);
        _mm256_store_si256((reg*) &b[8 * i + 128], r5);
        _mm256_store_si256((reg*) &b[8 * i + 160], r6);
        _mm256_store_si256((reg*) &b[8 * i + 192], r7);
        _mm256_store_si256((reg*) &b[8 * i + 224], r8);
    }
}

alignas(64) char a[SIZE], b[SIZE];
int buf_len = 0, buf_pos = 0;

void scalar_parse(int &s) {
    int x = 0;

    while (buf_pos < buf_len) {
        char c = a[buf_pos++];
        if (c >= '0' && c <= '9')
            x = 10 * x + (c - '0');
        else {
            s ^= x;
            x = 0;
        }
    }
}

void print(reg x) {
    int buf[8];
    _mm256_storeu_si256((reg*) buf, x);
    for (int i = 0; i < 8; i++)
        printf("%d ", buf[i]);
    printf("\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int parse_int(char *s) {
    int x = 0, k = 0;

    while (s[k] != '\n') {
        x = 10 * x + (s[k] - '0');
        k++;
    }

    return x;
}

// parses a full block
// returns remaining bytes
int simd_parse(int &_s) {
    reg s[K], x[K];

    for (int i = 0; i < K; i++) {
        transpose(a + i * (SIZE / K), b + i * (SIZE / K));
        s[i] = x[i] = _mm256_setzero_si256();
    }

    for (int i = 0; i < SIZE / K; i += 256) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 2; l++) {
                    for (int p = 0; p < K; p++) {
                        const reg zero = _mm256_set1_epi32('0');
                        const reg real_zero = _mm256_setzero_si256();
                        const reg ten = _mm256_set1_epi32(10);

                        reg c = _mm256_cvtepu8_epi32(
                            _mm_loadl_epi64((__m128i*) &b[p * (SIZE / K) + i + j * 16 + k * 32 + l * 8])
                        );

                        reg mask = _mm256_cmpgt_epi32(zero, c); // 1s are separators

                        c = _mm256_sub_epi32(c, zero);

                        reg y = _mm256_add_epi32(_mm256_mullo_epi32(x[p], ten), c);
                        s[p] = _mm256_xor_si256(s[p], _mm256_and_si256(x[p], mask));

                        x[p] = _mm256_blendv_epi8(y, real_zero, mask);
                    }
                }
            }
        }
    }

    for (int i = 1; i < K; i++)
        s[0] = _mm256_xor_si256(s[0], s[i]);

    int buf[8];
    _mm256_storeu_si256((reg*) buf, s[0]);

    for (int i = 0; i < 8; i++)
        _s ^= buf[i];

    // cancel some
    for (int i = 1; i < 8 * K; i++) {
        if (a[i * P - 1] != '\n') {
            _s ^= parse_int(&a[i * P]);

            int k = i * P - 1;
            while (a[k - 1] != '\n')
                k--;
            _s ^= parse_int(&a[k]);
        }
    }

    int rem = 0;

    while (a[SIZE - rem - 1] != '\n')
        rem++;

    return rem;
}

int read(int n) {
    // ignore n
    int s = 0;

    buf_len = fread(a, 1, SIZE, stdin);
    
    while (buf_len == SIZE) {
        int rem = simd_parse(s); // scalar_parse
        memcpy(a, a + (SIZE - rem), rem);
        buf_len = rem + fread(a + rem, 1, (SIZE - rem), stdin);
        //printf("%d %d %d\n", s, rem, buf_len);
    }
    
    scalar_parse(s);
    
    return s;
}
