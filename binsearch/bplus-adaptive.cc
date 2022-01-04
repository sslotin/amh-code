#include "binsearch.hh"
#include <x86intrin.h>
#include <sys/mman.h>

typedef __m256i reg;

const int INF = std::numeric_limits<int>::max();

class Layer {
    int size;
    bool permuted;
    bool prefetch;
};

// factory class?

template<int b, bool p>
unsigned rank(reg x, int* node) {
    // ...
}

template<>
unsigned rank<8, false>(reg x, int* node) {
    // ...
}

template<>
unsigned rank<16, false>(reg x, int* node) {
    reg a = _mm256_load_si256((reg*) node);
    reg b = _mm256_load_si256((reg*) (node + 8));

    reg ca = _mm256_cmpgt_epi32(a, x);
    reg cb = _mm256_cmpgt_epi32(b, x);

    int mb = _mm256_movemask_ps((__m256) cb);
    int ma = _mm256_movemask_ps((__m256) ca);
    
    unsigned mask = (1 << 16);
    mask |= mb << 8;
    mask |= ma;

    return __tzcnt_u32(mask);
}

template<>
unsigned rank<32, false>(reg x, int* node) {
    /*
    reg a = _mm256_load_si256((reg*) node);
    reg b = _mm256_load_si256((reg*) (node + 8));

    reg ca = _mm256_cmpgt_epi32(a, x);
    reg cb = _mm256_cmpgt_epi32(b, x);

    int mb = _mm256_movemask_ps((__m256) cb);
    int ma = _mm256_movemask_ps((__m256) ca);
    
    unsigned mask = (1 << 16);
    mask |= mb << 8;
    mask |= ma;

    return __tzcnt_u32(mask);
    */
}

template<int b>
void permute(int *node) {
    // ...
}

template<>
void permute<16>(int *node) {
    const reg perm_mask = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);
    reg* middle = (reg*) (node + 4);
    reg x = _mm256_loadu_si256(middle);
    x = _mm256_permutevar8x32_epi32(x, perm_mask);
    _mm256_storeu_si256(middle, x);
}

template<>
void permute<32>(int *node) {
    // ...
}

template<>
void rank<16, true>(reg x, int* node) {
    reg a = _mm256_load_si256((reg*) node);
    reg b = _mm256_load_si256((reg*) (node + 8));

    reg ca = _mm256_cmpgt_epi32(a, x);
    reg cb = _mm256_cmpgt_epi32(b, x);

    reg c = _mm256_packs_epi32(ca, cb);
    unsigned mask = _mm256_movemask_epi8(c);

    return __tzcnt_u32(mask)/* >> 1*/;
}

template<>
void rank<32, true>(reg x, int *node) {
    // ...
}

template<int... block_sizes>
struct STree {
    static constexpr int H = sizeof...(block_sizes); // structure height
    static constexpr int B[H] = {block_sizes...};    // block sizes at each level
    
    constexpr int blocks(int n, int b) {
        return (n + b - 1) / b;
    }
    
    constexpr int prev_keys(int n, int b) {
        return (blocks(n) + b) / (b + 1) * b;
    }

    constexpr int capacity() {
        // todo: fix me and fold me
        int cnt = B[0];
        
        for (int i = 1; i < H; i++)
            cnt *= (B[i] + 1);

        return cnt;
    }

    static_assert(capacity() >= N);

    constexpr int offset(int h) {
        // todo: validate
        // todo: small blocks
        int k = 0, n = N;
        while (h--) {
            k += blocks(n) * B;
            n = prev_keys(n);
        }
        return k;
    }

    int *t;

    STree(int *a/*, int n*/) {
        const int S = offset(H);               // non-empty memory
                  P = 1 << 21,                 // page size
                  M = (4 * S + P - 1) / P * P; // how much memory you actually need
        
        // todo: fallback for non-Linux systems
        t = (int*) std::aligned_alloc(P, M);
        madvise(btree, M, MADV_HUGEPAGE);

        // todo: smarter fill
        for (int i = N; i < S; i++)
            t[i] = INF;

        memcpy(t, a, 4 * N);
        
        for (int h = 1; h < H; h++) {
            for (int i = 0; i < offset(h + 1) - offset(h); i++) {
                int k = i / B[h],
                    j = i - k * B[h];
                k = k * (B[h] + 1) + j + 1; // compare right
                // and then always to the left
                for (int l = 0; l < h - 1; l++) // why (h - 1)?
                    k *= (B[l] + 1);
                btree[offset(h) + i] = (k * B[h] < N ? btree[k * B] : INF);
            }
        }

        for (int h = 1; h < H; h++) {
            permute(btree + i);
        }
    }

    int lower_bound(int _x) {
        unsigned k = 0;
        reg x = _mm256_set1_epi32(_x - 1);
        for (int h = H - 1; h > 0; h--) {
            unsigned i = permuted_rank(x, btree + offset(h) + k);
            
            //k /= B;
            //k *= (B + 1) * B;
            // k += (i << 3);
            
            k = k * (B + 1) + (i << 3);
            
            //if (N > (1 << 21) && h == 1)
            //    __builtin_prefetch(btree + k);
            
            //k += (i << 3);
        }
        unsigned i = direct_rank(x, btree + k);
        return btree[k + i];
    }
};

STree<16, 16> t;

void prepare(int *a, int n) {
    assert(n == N);
    t = STree<16, 16>(a);
}

int lower_bound(int _x) {
    unsigned k = 0;
    reg x = _mm256_set1_epi32(_x - 1);
    for (int h = H - 1; h > 0; h--) {
        unsigned i = permuted_rank(x, btree + offset(h) + k);
        
        //k /= B;
        //k *= (B + 1) * B;
        // k += (i << 3);
        
        k = k * (B + 1) + (i << 3);
        
        //if (N > (1 << 21) && h == 1)
        //    __builtin_prefetch(btree + k);
        
        //k += (i << 3);
    }
    unsigned i = direct_rank(x, btree + k);
    return btree[k + i];
}
