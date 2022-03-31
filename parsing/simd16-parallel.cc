#include "read.hh"

void prepare() {}

const int SIZE = (1<<16); // 16K
const int K = 4;
char buf[SIZE];
int buf_len = 0, buf_pos = 0;

// parse the rest of the buffer and output the answer
// returns number of bytes parsed
int scalar_parse(int &s) {
    int x = 0;
    int bytes_parsed = 0;

    while (buf_pos < buf_len) {
        char c = buf[buf_pos++];
        if (c >= '0' && c <= '9')
            x = 10 * x + (c - '0');
        else {
            s ^= x;
            x = 0;
            bytes_parsed = buf_pos;
        }
    }

    return bytes_parsed;
}

typedef __m128i reg;

void print(reg var) {
    char *val = (char*) &var;
    for (int i = 0; i < 16; i++)
        printf("%.2x ", val[i] & 0xff);
    printf("\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// input: 2x8 vector containing encoded digits
// output: 2-integer vector
reg convert(reg x) {
    // merge into 8 2-digit numbers, one byte each

    const reg mul10 = _mm_set1_epi16((1 << 8) + 10); // (1, 10, 1, 10...)
    x = _mm_maddubs_epi16(x, mul10);

    // merge into 4 4-digit numbers, two bytes each
    const reg mul100 = _mm_set1_epi32((1 << 16) + 100); // (1, 100, 1, 100...)
    x = _mm_madd_epi16(x, mul100);

    // pack numbers tighter (second half will be a copy of first; we don't need it anyway)
    x = _mm_packus_epi32(x, x);

    // merge into 2 8-digit numbers, 4 bytes each (and their copy on the right half)
    const reg mul10000 = _mm_set1_epi32((1 << 16) + 10000); // (1, 10000, 1, 10000...)
    x = _mm_madd_epi16(x, mul10000);

    return x;
}

// parses a full block
int simd_parse(int &_s) {
    reg s[K];
    int p[K];

    for (int i = 0; i < K; i++) {
        s[i] = _mm_setzero_si128();
        p[i] = i * (SIZE / K);
        // p[i] = TODO
    }

    p[0] = buf_pos;

    // buf_pos always points at the start of a number
    while (p[K - 1] + 15 < SIZE) {
        for (int i = 0; i < K; i++) {
            reg x = _mm_loadu_si128( (reg*) &buf[p[i]] );

            const reg zero = _mm_set1_epi8('0');
            reg mask = _mm_cmplt_epi8(x, zero);

            int m = _mm_movemask_epi8(mask); // zeros correspond to digits
            int d = __builtin_ffs(m); // relative position of first separator (1-indexed)

            p[i] += d;
            //printf("%d\n", d);

            // convert ASCII chars to 2x8 numbers 0..9, one byte each
            x = _mm_subs_epu8(x, zero);

            //cout << d << " " << (9 - d) << endl;
            //x = _mm_bslli_si128(x, )
            x = _mm_slli_epi64(x, 8 * (9 - d)); // shift right by (9 - d) bytes
            //print(x);
            x = convert(x);

            s[i] = _mm_xor_si128(s[i], x);
        }
    }

    for (int i = 0; i < K; i++)
        _s ^= _mm_cvtsi128_si32(s[i]);

    return p[K - 1];
}

int read(int n) {
    // ignore n
    int s = 0;

    while (true) {
        buf_len = buf_pos + fread(buf + buf_pos, 1, SIZE - buf_pos, stdin);
        buf_pos = 0;
        if (buf_len == SIZE) {
            int bytes_parsed = simd_parse(s); // scalar_parse
            //printf("%d %d %d\n", s, bytes_parsed, buf_len);
            buf_pos = SIZE - bytes_parsed;
            memcpy(buf, buf + bytes_parsed, buf_pos);
        } else {
            scalar_parse(s);
            break;
        }
    }

    return s;
}
