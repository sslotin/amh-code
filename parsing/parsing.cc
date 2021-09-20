#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

const int n = 1e7;

void stream() {
    int s = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        s ^= x;
    }

    cout << s << endl;
}

void stream2() {
    ios::sync_with_stdio(false);

    int s = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        s ^= x;
    }

    cout << s << endl;
}

void cstyle() {
    unsigned int s = 0;
    for (int i = 0; i < n; i++) {
        unsigned int x;
        scanf("%u", &x);
        s ^= x;
    }

    cout << s << endl;
}

/*
static const int buf_size = 4096;
static unsigned char buf[buf_size];
static int buf_len = 0, buf_pos = 0;

inline bool isEof() {
    if (buf_pos == buf_len) {
        buf_pos = 0;
        buf_len = fread(buf, 1, buf_size, stdin);
        if (buf_pos == buf_len)
            return true;
    }
    return false;
}

inline int getChar() { 
    return isEof() ? -1 : buf[buf_pos++];
}

inline int readChar() {
    int c = getChar();
    while (c != -1 && c <= 32)
        c = getChar();
    return c;
}


inline int readUInt() {
    int c = readChar(), x = 0;
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return x;
}

void kapel() {
    int s = 0;

    for (int i = 0; i < n; i++) {
        int x = readUInt();
        s ^= x;
    }

    cout << s << endl;
}*/

void chars() {
    while (getchar_unlocked() != -1);
}

void manual() {
    int s = 0, x = 0;

    while (true) {
        int c = getchar_unlocked();
        if (c >= '0' && c <= '9')
            x = 10 * x + (c - '0');
        else {
            if (x != 0)
                s ^= x;
            if (c == EOF)
                break;
            x = 0;
        }
    }

    cout << s << endl;
}

const int buf_size = (1<<20); // 4k
char buf[buf_size];
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
            if (x != 0)
                s ^= x;
            x = 0;
            bytes_parsed = buf_pos;
        }
    }

    return bytes_parsed;
}

/*
void timeit(void (*f)()) {
    clock_t start = clock();
    f();
    cout << double(clock() - start) / CLOCKS_PER_SEC << endl;
}*/

/*
// input: 4x8 vector containing ASCII encoded digits
// output: 4-integer vector
__m128i convert(__m256i ascii) {
    // convert ASCII chars to 8x4 numbers 0..9, one byte each
    const __m256i zero = _mm256_set1_epi8('0');
    const __m256i d1 = _mm256_subs_epu8(ascii, zero);

    // merge into 16 2-digit numbers, one byte each
    const __m256i mul10 = _mm256_set1_epi16(1 << 8 + 10); // (1, 10, 1, 10...)
    const __m256i d2 = _mm256_maddubs_epi16(d1, mul10);

    // merge into 8 4-digit numbers, two bytes each
    const __m256i mul100 = _mm256_set1_epi32(1 << 8 + 100); // (1, 100, 1, 100...)
    const __m256i d4 = _mm256_madd_epi16(d2, mul100);

    // pack numbers tighter (data will be doubled though)
    const __m256i d4_doubled = _mm256_packus_epi32(d4, d4);

    // extract first half (instruction not needed: xmm_i is the first half of ymm_i)
    const __m128i d4_half = _mm256_castsi256_si128(d4_doubled);

    // merge into 4 8-digit numbers, 4 bytes each
    const __m128i mul10000 = _mm_set1_epi32(1 << 16 + 10000); // (1, 10000, 1, 10000...)
    const __m128i d8 = _mm_madd_epi16(d4_half, mul10000);

    return d8;
}

void precalc(__m128i x) {
    int mask = _mm_movemask_epi8(x);
    int d = __builtin_ffs(mask);
    // shift first by 8 * d
    // _mm_insert_epi64
    // _mm256_inserti128_si256
}

// buf_len=buf_size
void simd_parse(int &s) {
    __m128i s = _mm_setzero_si128();

    // while we guaranteed to parse at least 4 numbers
    while (buf_pos + 9*4 <= buf_size) {
        __m128i x[4];
        for (int i = 0; i < 4; i++) {
            __m256i t = _mm_load_si128((reg*) buf + buf_pos);
            // load 16 bytes
            // make a mask
            // fetch precalced shuffle mask
            x[i] = // apply it
            // jump to next digit
        }
        // convert 4 8-char blocks to 
        // xor it with accumulator
    }
}
*/

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
    //print(x);

    // merge into 8 2-digit numbers, one byte each
    const reg mul10 = _mm_set1_epi16((1 << 8) + 10); // (1, 10, 1, 10...)
    x = _mm_maddubs_epi16(x, mul10);

    //print(mul10);
    //print(x);

    // merge into 4 4-digit numbers, two bytes each
    const reg mul100 = _mm_set1_epi32((1 << 16) + 100); // (1, 100, 1, 100...)
    x = _mm_madd_epi16(x, mul100);

    //print(x);

    // pack numbers tighter (second half will be a copy of first; we don't need it anyway)
    x = _mm_packus_epi32(x, x);

    //print(x);

    // merge into 2 8-digit numbers, 4 bytes each (and their copy on the right half)
    const reg mul10000 = _mm_set1_epi32((1 << 16) + 10000); // (1, 10000, 1, 10000...)
    x = _mm_madd_epi16(x, mul10000);

    //print(x);

    return x;
}

// buf_len=buf_size
int simd_parse(int &_s) {
    reg s = _mm_setzero_si128();

    // buf_pos always points at the start of a number
    while (buf_pos + 15 < buf_size) {
        reg x = _mm_loadu_si128( (reg*) &buf[buf_pos] );

        const reg zero = _mm_set1_epi8('0');
        reg mask = _mm_cmplt_epi8(x, zero);
        // convert ASCII chars to 2x8 numbers 0..9, one byte each
        x = _mm_subs_epu8(x, zero);

        int m = _mm_movemask_epi8(mask); // zeros correspond to digits
        int d = __builtin_ffs(m); // relative position of first separator (1-indexed)

        buf_pos += d;

        //cout << d << " " << (9 - d) << endl;
        x = _mm_slli_epi64(x, 8 * (9 - d)); // shift right by (9 - d) bytes
        //print(x);
        x = convert(x);
        //cout << _mm_extract_epi32(x, 0) << endl << endl;
        
        s = _mm_xor_si128(s, x);
    }

    _s ^= _mm_extract_epi32(s, 0);

    return buf_pos;
}

void full_parse() {
    int s = 0;

    while (true) {
        buf_len = buf_pos + fread(buf + buf_pos, 1, buf_size - buf_pos, stdin);
        buf_pos = 0;
        if (buf_len == buf_size) {
            int bytes_parsed = simd_parse(s);
            buf_pos = buf_size - bytes_parsed;
            memcpy(buf, buf + bytes_parsed, buf_pos);
        } else {
            scalar_parse(s);
            break;
        }
    }

    cout << s << endl;
}

int main() {
    //reg t = _mm_set1_epi8(1);
    //reg t = _mm_setr_epi8(1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8);
    //cout << _mm_extract_epi32(convert(t), 0) << endl;

    manual();
    //full_parse();
    //chars();

    return 0;
}
