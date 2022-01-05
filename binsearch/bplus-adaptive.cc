#include "binsearch.hh"
#include <x86intrin.h>
#include <sys/mman.h>

typedef __m256i reg;

const int INF = std::numeric_limits<int>::max(); // type-specific

/*
template <int b>
unsigned rank(reg x, int *node) {
    int scalar = _mm256_extract_epi32(x, 0); // <- may be inefficient
    for (int i = 0; i < b; i++)
        if (node[i] > scalar)
            return i;
    return b;
}
*/

unsigned cmp(reg x, int *node) {
    reg y = _mm256_load_si256((reg*) node);
    reg mask = _mm256_cmpgt_epi32(y, x);
    return _mm256_movemask_ps((__m256) mask);
}

unsigned rank8(reg x, int *node) {
    unsigned mask = (1 << 8) | cmp(x, node);
    return __tzcnt_u32(mask);
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
    permute16(node);
    permute16(node + 16);
    reg x = _mm256_load_si256((reg*) (node + 8));
    reg y = _mm256_load_si256((reg*) (node + 16));
    _mm256_storeu_si256((reg*) (node + 8), y);
    _mm256_storeu_si256((reg*) (node + 16), x);
}

unsigned permuted_rank16(reg x, int *node) {
    reg a = _mm256_load_si256((reg*) node);
    reg b = _mm256_load_si256((reg*) (node + 8));

    reg ca = _mm256_cmpgt_epi32(a, x);
    reg cb = _mm256_cmpgt_epi32(b, x);

    reg c = _mm256_packs_epi32(ca, cb);
    unsigned mask = _mm256_movemask_epi8(c);

    return __tzcnt_u32(mask) >> 1; // handle shifts?
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

    return __tzcnt_u32(mask) >> 1;
}

struct Layer {
    int size;      // size of blocks on cache layer
    bool permuted; // whether the blocks there are permuted (3-4 cycles faster)
    bool prefetch; // prefetch the first possible child of the next layer
                   //  (not necessarily belonging to the cache line we are going to read)

    constexpr Layer() : size(8), permuted(false), prefetch(false) {}

    constexpr Layer(int size, bool permuted, bool prefetch)
                  : size(size), permuted(permuted), prefetch(prefetch) {}

    // I have no idea how to do this kind of compile-time polymorphism in C++
    // so let's do this and rely on compiler optimization
    void permute(int *node) const {
        //static_assert(permuted);
        if (size == 16)
            permute16(node);
        else
            permute32(node);
    };

    unsigned rank(reg x, int *node) const {
        if (permuted) {
            if (size == 16)
                return permuted_rank16(x, node);
            else // (size == 32)
                return permuted_rank32(x, node);
        } else {
            if (size == 8)
                return rank8(x, node);
            else if (size == 16)
                return rank16(x, node);
            else // (size == 32)
                return rank32(x, node);
        }
    }
};

template<int... block_sizes>
struct STree {
    // todo: some compile-time computation of optimal sizes

    static constexpr int H = sizeof...(block_sizes); // structure height
    static constexpr int B[H] = {block_sizes...};    // block sizes at each level

    struct Architect {
        Layer L[H];
        
        constexpr Architect() : L{} {
            for (int i = 0; i < H; i++)
                L[i] = {B[i], (i < H - 1), false};
        }    
    };

    static constexpr Architect A;

    // number of full blocks in a layer
    constexpr int blocks(int n, int b) {
        return (n + b - 1) / b;
    }
    
    // number of keys on the previous layer
    // n: number of blocks on the current layer
    // b: block size on previous layer
    constexpr int prev_keys(int n, int b) {
        // get the number of blocks (considering that each block is responsible
        // for (b + 1) elements and not b), and then calculate the block size
        return (n + b) / (b + 1) * b;
    }

    // the maximum supported dataset size
    // currently used just for compile-time checking
    constexpr int capacity() {
        // maybe this could be folded with std::accumulate
        int cnt = B[0];
        for (int i = 1; i < H; i++)
            cnt *= (B[i] + 1);
        return cnt;
    }

    // offset in the memory array for the layer "h" (0th is the array itself)
    constexpr int offset(int h) {
        int k = 0, n = N;
        for (int l = h; l >= 0; l--) {
            int m = blocks(n, B[l]);
            k += ((m * B[l]) + 15) / 16 * 16; // ceiled to cache-align the next layer
            if (l > 0)
                n = prev_keys(m, B[l - 1]);
        }
        return k;
    }

    int *t;

    STree() {}

    STree(int *a/*, int n*/) {
        const int S = offset(H),               // non-empty memory
                  P = 1 << 21,                 // page size
                  M = (4 * S + P - 1) / P * P; // how much memory you actually need
        
        // todo: fallback for non-Linux systems
        t = (int*) std::aligned_alloc(P, M);
        madvise(t, M, MADV_HUGEPAGE);

        // todo: more efficient fill
        for (int i = N; i < S; i++)
            t[i] = INF;

        memcpy(t, a, 4 * N);
        
        for (int h = 1; h < H; h++) {
            for (int i = 0; i < offset(h + 1) - offset(h); i++) {
                int k = i / B[h],
                    j = i - k * B[h];
                k = k * (B[h] + 1) + j + 1; // compare right
                // and then always to the left
                for (int l = 1; l < h; l++)
                    k *= (B[l] + 1);
                k *= B[0];
                t[offset(h) + i] = (k < N ? t[k] : INF);
            }
        }

        for (int h = 0; h < H; h++)
            if (A.L[h].permuted)
                for (int i = offset(h); i < offset(h + 1); i += B[h])
                    A.L[h].permute(t + i);
    }

    // we need lower_bound, but upper bound is just easier to implement
    int upper_bound(int _x) {
        unsigned k = 0;
        reg x = _mm256_set1_epi32(_x);
        for (int h = H - 1; h > 0; h--) {
            unsigned i = A.L[h].rank(x, t + offset(h) + k);
            // compiler may need help with optimizing shifts here
            k = k * (B[h] + 1) + i * B[h - 1];
            //if (N > (1 << 21) && h == 1)
            //    __builtin_prefetch(t + k);
        }
        unsigned i = A.L[0].rank(x, t + k);
        return t[k + i]; // return index or a pair?
    }
};

typedef STree<16, 16> Tree;
Tree t;

static_assert(t.capacity() >= N);
// todo: perhaps lower bound too?

void prepare(int *a, int n) {
    assert(n == N);
    t = Tree(a);
}

int lower_bound(int x) {
    return t.upper_bound(x - 1);
}
