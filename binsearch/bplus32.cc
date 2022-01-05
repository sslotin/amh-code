#include "binsearch.hh"
#include <x86intrin.h>
#include <sys/mman.h>

typedef __m256i reg;

const int B = 32, INF = std::numeric_limits<int>::max();

constexpr int blocks(int n) {
    return (n + B - 1) / B;
}

constexpr int prev_keys(int n) {
    return (blocks(n) + B) / (B + 1) * B;
}

constexpr int height(int n) {
    return (n <= B ? 1 : height(prev_keys(n)) + 1);
}

constexpr int offset(int h) {
    int k = 0, n = N;
    while (h--) {
        k += blocks(n) * B;
        n = prev_keys(n);
    }
    return k;
}

const int H = height(N), S = offset(H);

int *btree;

void permute16(int *node) {
    // a b c d -> a c b d
    const reg perm_mask = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);
    reg* middle = (reg*) (node + 4);
    reg x = _mm256_loadu_si256(middle);
    x = _mm256_permutevar8x32_epi32(x, perm_mask);
    _mm256_storeu_si256(middle, x);
}

void permute32(int *node) {
    // a b c d 1 2 3 4 -> (a c) (b d) (1 3) (2 4) -> (a c) (1 3) (b d) (2 4)
    reg x = _mm256_load_si256((reg*) (node + 8));
    reg y = _mm256_load_si256((reg*) (node + 16));
    _mm256_storeu_si256((reg*) (node + 8), y);
    _mm256_storeu_si256((reg*) (node + 16), x);
    permute16(node);
    permute16(node + 16);
}

void prepare(int *a, int n) {
    const int P = 1 << 21, T = (4 * S + P - 1) / P * P;
    btree = (int*) std::aligned_alloc(P, T);
    madvise(btree, T, MADV_HUGEPAGE);

    for (int i = N; i < S; i++)
        btree[i] = INF;

    memcpy(btree, a, 4 * N);
    
    for (int h = 1; h < H; h++) {
        for (int i = 0; i < offset(h + 1) - offset(h); i++) {
            int k = i / B,
                j = i - k * B;
            k = k * (B + 1) + j + 1; // compare right
            // and then always to the left
            for (int l = 0; l < h - 1; l++)
                k *= (B + 1);
            btree[offset(h) + i] = (k * B < N ? btree[k * B] : INF);
        }
    }

    for (int i = offset(1); i < S; i += B)
        permute32(btree + i);
}


unsigned cmp(reg x, int *node) {
    reg y = _mm256_load_si256((reg*) node);
    reg mask = _mm256_cmpgt_epi32(y, x);
    return _mm256_movemask_ps((__m256) mask);
}

unsigned rank32(reg x, int *node) {
    unsigned mask = cmp(x, node)
                  | (cmp(x, node + 8) << 8)
                  | (cmp(x, node + 16) << 16)
                  | (cmp(x, node + 24) << 24);
    return __tzcnt_u32(mask);    
}

unsigned permuted_rank32(reg x, int *node) {
    reg a = _mm256_load_si256((reg*) node);
    reg b = _mm256_load_si256((reg*) (node + 8));
    reg c = _mm256_load_si256((reg*) (node + 16));
    reg d = _mm256_load_si256((reg*) (node + 24));

    reg ca = _mm256_cmpgt_epi32(a, x);
    reg cb = _mm256_cmpgt_epi32(b, x);
    reg cc = _mm256_cmpgt_epi32(c, x);
    reg cd = _mm256_cmpgt_epi32(d, x);

    reg cab = _mm256_packs_epi32(ca, cb);
    reg ccd = _mm256_packs_epi32(cc, cd);
    reg cabcd = _mm256_packs_epi16(cab, ccd);
    unsigned mask = _mm256_movemask_epi8(cabcd);

    return __tzcnt_u32(mask);
}

int lower_bound(int _x) {
    unsigned k = 0;
    reg x = _mm256_set1_epi32(_x - 1);
    for (int h = H - 1; h > 0; h--) {
        unsigned i = permuted_rank32(x, btree + offset(h) + k);
        
        //k /= B;
        //k *= (B + 1) * B;
        // k += (i << 3);
        
        k = k * (B + 1) + (i << 5); // <- I've spent 2 hours finding this bug
        
        //if (N > (1 << 21) && h == 1)
        //    __builtin_prefetch(btree + k);
        
        //k += (i << 3);
    }
    unsigned i = rank32(x, btree + k);
    return btree[k + i];
}
