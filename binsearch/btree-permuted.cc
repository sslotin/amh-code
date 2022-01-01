#include "binsearch.hh"
#include <x86intrin.h>
#include <sys/mman.h>

typedef __m256i reg;

const int B = 16;
const int INF = std::numeric_limits<int>::max();

constexpr int height(int n) {
    int s = 0, // total size
        l = B, // size of next layer
        h = 0; // height so far
    while (s + l - B < n) {
        s += l;
        l *= (B + 1);
        h++;
    }
    return h;
}

const int nblocks = (N + B - 1) / B;
int *_a;
int (*btree)[B];

int blank[B] = {-1};

int go(int k, int i) { return k * (B + 1) + i + 1; }

void permute(int *node) {
    // todo: replace with _mm256_permutevar_pd
    const reg perm_mask = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);
    reg* middle = (reg*) (node + 4);
    // we can't load aligned load here because it explicitly crosses cache boundary
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
    //n = _n;
    //nblocks = (n + B - 1) / B;
    _a = a;
    btree = (int(*)[16]) std::aligned_alloc(2 * 1024 * 1024, 64 * nblocks);
    madvise(btree, 64 * nblocks, MADV_HUGEPAGE);
    build();
}

int cmp(reg x_vec, int* y_ptr) {
    reg y_vec = _mm256_load_si256((reg*) y_ptr);
    reg mask = _mm256_cmpgt_epi32(x_vec, y_vec);
    return _mm256_movemask_ps((__m256) mask);
}

int old_get_idx(reg x_vec, int* y_ptr) {
    int mask = ~(
        cmp(x_vec, y_ptr) +
        (cmp(x_vec, y_ptr + 8) << 8)
    );
    return __builtin_ffs(mask) - 1;
}

int get_idx(reg x_vec, int* y_ptr) {
    //std::cerr << _mm256_extract_epi32(x_vec, 0) << std::endl;
    //permute(y_ptr);
    //int j = old_get_idx(x_vec, y_ptr);
    //permute(y_ptr);

    //const reg ones = _mm256_set1_epi32(1);
    //x_vec = _mm256_sub_epi32(x_vec, ones);

    /*
    std::cerr << _mm256_extract_epi32(x_vec, 0) << std::endl;
    
    for (int i = 0; i < B; i++)
        std::cerr << y_ptr[i] << " ";

    std::cerr << std::endl;
    */

    reg a = _mm256_load_si256((reg*) y_ptr);
    reg b = _mm256_load_si256((reg*) (y_ptr + 8));

    reg ca = _mm256_cmpgt_epi32(a, x_vec);
    reg cb = _mm256_cmpgt_epi32(b, x_vec);

    //const reg perm_mask = _mm256_set_epi32(7, 6, 3, 2, 5, 4, 1, 0);
    reg c = _mm256_packs_epi32(ca, cb);
    //c = _mm256_permutevar8x32_epi32(c, perm_mask);
    //permute(y_ptr);

    int mask = _mm256_movemask_epi8(c);
    
    //int i = (__builtin_ffs(mask) >> 1) - 1;
    int i = _tzcnt_u32(mask) >> 1;
    //__tzcnt_u32();
    //std::cerr << i << " " << j << std::endl << std::endl;
    
    //if (i != j)
    //    exit(0);
    //assert(i == j);

    return i;
}

int transform(int i) {
    return (i & 0b0011) | ((i & 0b1000) >> 1) | ((i & 0b0100) << 1);
}

void update(int &res, int* node, int i) {
    const int translate[16] = {
        0, 1, 2, 3,
        8, 9, 10, 11,
        4, 5, 6, 7,
        12, 13, 14, 15
    };
    // 2 lowest bits are preserved
    // >>2 = {0, 2, 1, 3}
    // swap high and low bits

    //for (int k = 0; k < 16; k++)
    //    std::cerr << transform(k) << " " << translate[k] << std::endl;

    //res = i;
    if (i < B) [[likely]]
        //res = node[transform(i)];
        res = node[translate[i]];
}

int lower_bound(int x) {
    //std::cerr << 123 << std::endl;
    int k = 0, res = INF;
    reg x_vec = _mm256_set1_epi32(x - 1);
    for (int h = 0; h < height(N); h++) {
        int i = get_idx(x_vec, btree[k]);
        update(res, btree[k], i);
        k = go(k, i);
    }
    if (k < nblocks) {
        int i = get_idx(x_vec, btree[k]);
        update(res, btree[k], i);
    }
    return res;
}
