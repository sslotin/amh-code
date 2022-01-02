#include "binsearch.hh"
#include <x86intrin.h>
#include <sys/mman.h>

typedef __m256i reg;

const int B = 16, INF = std::numeric_limits<int>::max();

void print(int* node) {
    for (int i = 0; i < B; i++)
        std::cerr << node[i] << " ";
    std::cerr << std::endl;
}

void print(int x) {
    std::cerr << x << std::endl;
}

constexpr int blocks(int n) {
    return (n + B - 1) / B;
}

constexpr int prev_keys(int n) {
        // 1  -> 0  (16)
        // 2  -> 1  (16)
        // 16 -> 15 (16)
        // 17 -> 16 (16)
        // 18 -> 16 (32)
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

void permute(int *node) {
    const reg perm_mask = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);
    reg* middle = (reg*) (node + 4);
    reg x = _mm256_loadu_si256(middle);
    x = _mm256_permutevar8x32_epi32(x, perm_mask);
    _mm256_storeu_si256(middle, x);
}

void prepare(int *a, int n) {
    const int P = 1 << 21, T = (4 * S + P - 1) / P * P;
    btree = (int*) std::aligned_alloc(P, T);
    madvise(btree, T, MADV_HUGEPAGE);

    for (int i = N; i < S; i++)
        btree[i] = INF;

    //for (int i = 0; i < std::min(S + 256, T / 4); i++)
    //    btree[i] = INF;

    //for (int i = 0; i < N; i++)
    //    std::cerr << a[i] << " ";
    //std::cerr << std::endl;

    memcpy(btree, a, 4 * N);
    
    //std::cerr << btree[16] << " " << a[16] << " " << INF << " " << n << " " << N << std::endl;
    
    for (int h = 1; h < H; h++) {
        for (int i = 0; i < offset(h + 1) - offset(h); i++) {
            int k = i / B,
                j = i - k * B;
            k = k * (B + 1) + j + 1; // compare right
            // and then always to the left
            for (int l = 0; l < h - 1; l++)
                k *= (B + 1);
            btree[offset(h) + i] = (k * B < N ? btree[k * B] : INF);
            //std::cerr << offset(h) + i << " <- " << k * B << " : " << btree[offset(h) + i] << std::endl;
            //assert(k >= offset(h) && k < offset(h + 1) - 1);
            //if (((i - offset(h - 1)) / B) % (B + 1) != 0) {
                //assert(btree[k] == INF); // no overwrites
                //btree[k++] = btree[i];
                //std::cerr << btree[i] << " " << i << " -> " << k - 1 << " / " << offset(h + 1) << std::endl;
            //}
        }
    }

    //exit(0);

    for (int i = offset(1); i < S; i += B)
        permute(btree + i);
}

int cmp(reg x_vec, int* y_ptr) {
    reg y_vec = _mm256_load_si256((reg*) y_ptr);
    reg mask = _mm256_cmpgt_epi32(y_vec, x_vec);
    return _mm256_movemask_ps((__m256) mask);
}

unsigned naive_rank(reg x_vec, int* y_ptr) {
    for (int i = 0; i < B; i++)
        if (y_ptr[i] > _mm256_extract_epi32(x_vec, 0))
            return i;
    return B;
}

unsigned direct_rank(reg x_vec, int* y_ptr) {
    //return naive_rank(x_vec, y_ptr);
    unsigned mask = (
        (1 << 16) |
        cmp(x_vec, y_ptr) |
        (cmp(x_vec, y_ptr + 8) << 8)
        // rearrange?
    );
    unsigned i = __tzcnt_u32(mask);
    //print(y_ptr);
    //print(i);
    return i;
}

unsigned permuted_rank(reg x_vec, int* y_ptr) {
    /*
    permute(y_ptr);
    unsigned i = naive_rank(x_vec, y_ptr);
    permute(y_ptr);
    return i;
    */

    reg a = _mm256_load_si256((reg*) y_ptr);
    reg b = _mm256_load_si256((reg*) (y_ptr + 8));

    reg ca = _mm256_cmpgt_epi32(a, x_vec);
    reg cb = _mm256_cmpgt_epi32(b, x_vec);

    reg c = _mm256_packs_epi32(ca, cb);
    unsigned mask = _mm256_movemask_epi8(c);

    return __tzcnt_u32(mask) >> 1;    
}

int lower_bound(int x) {
    std::cerr << std::endl;
    std::cerr << "btree: " << S << " " << H << std::endl;
    /*
    for (int i = 0; i < S; i += B)
        print(btree + i);
    
    print(x);
    */

    int k = 0;
    reg x_vec = _mm256_set1_epi32(x - 1);
    for (int h = H - 1; h > 0; h--) {
        unsigned i = permuted_rank(x_vec, btree + offset(h) + k * B);
        //std::cerr << i << std::endl;
        //print(btree + offset(h) + k * B);
        k *= (B + 1);
        k += i;
        //if (h > 3)
        //    __builtin_prefetch(&btree[go(k, 0)]);
        //std::cerr << i << " " << k << " " << (offset(h) - offset(h - 1)) / B << std::endl;
    }
    //assert(k * B < S);
    unsigned i = direct_rank(x_vec, btree + k * B);
    //print(btree + k * B);
    //print(btree + k * B + B);
    return btree[k * B + i];
}
