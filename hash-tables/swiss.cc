#include "hashmap.hh"
#include <x86intrin.h>

const float alpha = 0.75;

int filled = 0, capacity = 1024;

struct element { int x, y; };
element *t;
char *h;

char* alloc(int n) {
    char* ptr = (char*) std::aligned_alloc(32, n);
    memset(ptr, -1, n); 
    return ptr;
}

void setup() {
    t = new element[capacity];
    h = alloc(capacity);
}

void rehash() {
    cerr << "rehash " << capacity << endl;

    char *_h = h;
    element *_t = t;

    filled = 0;
    capacity *= 2;
    
    t = new element[capacity];
    h = alloc(capacity);

    for (int i = 0; i < capacity / 2; i++)
        if (_h[i] != -1)
            add(_t[i].x, _t[i].y);

    delete _h;
    delete _t;
}

int pos(int k) { return k & (capacity - 1); }

const auto zs = _mm256_set1_epi8(-1);

void add(int x, int y) {
    if (filled + 1 > alpha * capacity)
        rehash();
    filled++;

    // 32 = 2^5 elements per block
    int k = (x & (capacity - 1)) / 32 * 32;
    char g = x >> (32 - 8); // [0, 128), upper bit is always 0
    
    auto gs = _mm256_set1_epi8(g);

    while (true) {
        //cerr << "add " << x << " " << y << " : " << k << endl;
        auto hs = _mm256_load_si256((__m256i*) &h[k]);
        
        int mask_equal = _mm256_movemask_epi8(_mm256_cmpeq_epi8(hs, gs));
        while (mask_equal != 0) {
            int idx = k + __builtin_ctz(mask_equal);
            if (t[idx].x == x) {
                t[idx].y = y;
                return;
            }
            mask_equal &= (mask_equal - 1);
        }
        
        int mask_empty = _mm256_movemask_epi8(_mm256_cmpeq_epi8(hs, zs));
        if (mask_empty != 0) {
            int idx = k + __builtin_ctz(mask_empty);
            //cerr << "add " << k << " " << idx << endl;
            h[idx] = g;
            t[idx] = {x, y};
            //cerr << x << " " << y << " : " << t[idx].x << " " << t[idx].y << endl; 
            return;
        }

        k = pos(k + 32);
    }
}

int get(int x) {
    //cerr << "get " << x << endl;
    int k = pos(x) / 32 * 32;
    char g = x >> (32 - 8);
    auto gs = _mm256_set1_epi8(g);

    while (true) {
        auto hs = _mm256_load_si256((__m256i*) &h[k]);
        
        int mask_equal = _mm256_movemask_epi8(_mm256_cmpeq_epi8(hs, gs));
        while (mask_equal != 0) {
            int idx = k + __builtin_ctz(mask_equal);
            if (t[idx].x == x) {
                //cerr << "yes" << endl;
                return t[idx].y;
            }
            //cerr << "false positive" << endl;
            mask_equal &= (mask_equal - 1);
        }
        
        int mask_empty = _mm256_movemask_epi8(_mm256_cmpeq_epi8(hs, zs));
        if (mask_empty != 0) {
            //cerr << "no" << endl;
            return -1;
        }

        k = pos(k + 32);
    }
}
