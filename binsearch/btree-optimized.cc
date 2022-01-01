#include "binsearch.hh"
#include <x86intrin.h>
#include <sys/mman.h>

typedef __m256i reg;

const int B = 16;
const int INF = std::numeric_limits<int>::max();

constexpr std::pair<int, int> precalc(int n) {
    int s = 0, // total size
        l = B, // size of next layer
        h = 0; // height so far
    while (s + l - B < n) {
        s += l;
        l *= (B + 1);
        h++;
    }
    int r = (n - s + B - 1) / B; // remaining blocks on last layer
    return {h, s / B + (r + B) / (B + 1) * (B + 1)};
}

const int height = precalc(N).first, nblocks = precalc(N).second;
int *_a, (*btree)[B];

int go(int k, int i) { return k * (B + 1) + i + 1; }

void permute(int *node) {
    const reg perm_mask = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);
    reg* middle = (reg*) (node + 4);
    reg x = _mm256_loadu_si256(middle);
    x = _mm256_permutevar8x32_epi32(x, perm_mask);
    _mm256_storeu_si256(middle, x);
}

void build(int k = 0) {
    static int t = 0;
    if (k < nblocks) {
        for (int i = 0; i < B; i++) {
            build(go(k, i));
            btree[k][i] = (t < N ? _a[t++] : INF);
        }
        permute(btree[k]);
        build(go(k, B));
    }
}

void prepare(int *a, int _n) {
    _a = a;
    btree = (int(*)[16]) std::aligned_alloc(2 * 1024 * 1024, 64 * nblocks);
    madvise(btree, 64 * nblocks, MADV_HUGEPAGE);
    build();
}

int rank(reg x_vec, int* y_ptr) {
    reg a = _mm256_load_si256((reg*) y_ptr);
    reg b = _mm256_load_si256((reg*) (y_ptr + 8));

    reg ca = _mm256_cmpgt_epi32(a, x_vec);
    reg cb = _mm256_cmpgt_epi32(b, x_vec);

    reg c = _mm256_packs_epi32(ca, cb);
    int mask = _mm256_movemask_epi8(c);

    return _tzcnt_u32(mask) >> 1;    
}

const int translate[17] = {
    0, 1, 2, 3,
    8, 9, 10, 11,
    4, 5, 6, 7,
    12, 13, 14, 15,
    0
};

void update(int &res, int* node, int i) {
    int val = node[translate[i]];
    res = (i < B ? val : res);
}

int lower_bound(int x) {
    int k = 0, res = INF;
    reg x_vec = _mm256_set1_epi32(x - 1);
    for (int h = 0; h < height - 1; h++) {
        int i = rank(x_vec, btree[k]);
        if (h > 3)
            __builtin_prefetch(&btree[go(k, 0)]);
        update(res, btree[k], i);
        k = go(k, i);
    }
    int i = rank(x_vec, btree[k]);
    update(res, btree[k], i);
    int k2 = go(k, i);
    if (height > 4)
        __builtin_prefetch(&btree[go(k, 0)]);
    if (go(k, 0) < nblocks) {
        int i = rank(x_vec, btree[k2]);
        update(res, btree[k2], i);
    }
    return res;
}
