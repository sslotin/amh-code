#include "btree.hh"
#include <x86intrin.h>

const int B = 32;
typedef __m256i reg;

unsigned cmp(reg x, int *node) {
    reg y = _mm256_load_si256((reg*) node);
    reg mask = _mm256_cmpgt_epi32(y, x);
    return _mm256_movemask_ps((__m256) mask);
}

unsigned rank16(reg x, int *node) {
    unsigned mask = (1 << 16) | (cmp(x, node + 8) << 8) | cmp(x, node);
    return __tzcnt_u32(mask);
}

unsigned rank32(reg x, int *node) {
    unsigned mask = cmp(x, node)
                  | (cmp(x, node + 8) << 8)
                  | (cmp(x, node + 16) << 16)
                  | (cmp(x, node + 24) << 24);
    return __tzcnt_u32(mask);    
}

int H = 1; // tree height

int *tree; // 31 (+ 1) + 32
int *data; // 31

int pos_tree = 0, pos_data = B;

int root = 0;

struct Precalc {
    alignas(64) int mask[B][B];

    constexpr Precalc() : mask{} {
        for (int i = 0; i < B; i++)
            for (int j = 0; j < B; j++)
                mask[i][j] = (i + j < B ? -1 : 0);
    }
};

constexpr Precalc P;

void prepare() {
    // pre-allocate a large memory? that's it?
}

// returns either {-1, -1} or new keys to add before i
// use flags?
std::pair<int, int> add_down(int k, int _x, reg x) {
    // check leaf?
    unsigned i = rank32(x, &tree[k]);
    k = tree[k + B + i];
    auto [l, r] = add_down(k, _x, x);
    
    if (l != -1) {
        for (int d = 0; d < B; d += 8) {
            // load mask? make sure it is stored in register? maybe just switch to scalar since this is rare?
            reg x = _mm256_loadu_si256(tree + d);
            _mm256_maskstore_epi32(tree + k + d, P.mask[i], x);
        }
    }
}

void add(int _x) {
    reg x = _mm256_set1_epi32(_x - 1);
    auto [l, r] = add_down(root, _x, x);
    // insert new root, increase height?
}

int lower_bound(int _x) {
    unsigned k = root;
    reg x = _mm256_set1_epi32(_x - 1);
    
    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &data[k]);

    return data[k + i]; // what if next block? maybe we store 31 elements?
}
