// you can use this implementation as-is if you have AVX2

// replace this header with <climits>:
#include "btree.hh"
#include <x86intrin.h>

const int B = 32;
const int R = 1e8; // reserve

alignas(64) int tree[R];
int root = 0, n_tree = B; // 30 + 31 for internal nodes and 31 for data nodes
int H = 1; // tree height

void prepare() {
    for (int i = 0; i < R; i++)
        tree[i] = INT_MAX;
}

typedef __m256i reg;

/*
reg cmp(reg x, int *node) {
    reg y = _mm256_load_si256((reg*) node);
    return _mm256_cmpgt_epi32(x, y);
}

unsigned rank32(reg x, int *node) {
    reg m1 = cmp(x, node);
    reg m2 = cmp(x, node + 8);
    reg m3 = cmp(x, node + 16);
    reg m4 = cmp(x, node + 24);

    m1 = _mm256_blend_epi16(m1, m2, 0b01010101);
    m3 = _mm256_blend_epi16(m3, m4, 0b01010101);
    m1 = _mm256_packs_epi16(m1, m3);

    unsigned mask = _mm256_movemask_epi8(m1);
    return __builtin_popcount(mask);    
}
*/

/*
unsigned rank32(int x, int *node) {
    unsigned base = 0, len = B;
    while (len > 1) { // unrolled?
        int half = len / 2;
        base = (node[base + half] < x ? base + half : base);
        len -= half;
    }
    return base + (node[base] < x);
}
*/

unsigned rank32(int x, int *node) {
    for (unsigned i = 0; i < B - 1; i++)
        if (node[i] >= x)
            return i;
    return B - 1;
}

int lower_bound(int x) {
    unsigned k = root;
    
    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &tree[k]);

    return tree[k + i];
}

struct Precalc {
    alignas(64) int mask[B][B];

    constexpr Precalc() : mask{} {
        for (int i = 0; i < B; i++)
            for (int j = i; j < B - 1; j++)
                mask[i][j] = -1;
    }
};

constexpr Precalc P;

void insert(int *node, int i, int x) {
    for (int j = B - 8; j >= 0; j -= 8) {
        reg t = _mm256_load_si256((reg*) &node[j]);
        reg mask = _mm256_load_si256((reg*) &P.mask[i][j]);
        _mm256_maskstore_epi32(&node[j + 1], mask, t);
    }
    node[i] = x;
}

// move the second half of a node and fill it with infinities
void move(int *from, int *to) {
    const reg infs = _mm256_set1_epi32(INT_MAX);
    for (int i = 0; i < B / 2; i += 8) {
        reg t = _mm256_load_si256((reg*) &from[B / 2 + i]);
        _mm256_store_si256((reg*) &to[i], t);
        _mm256_store_si256((reg*) &from[B / 2 + i], infs); // probably not necessary for pointers
    }
}

void insert(int x) {
    unsigned sk[10], si[10]; // stack
    
    unsigned k = root;
    
    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);

        tree[k + i] = (x > tree[k + i] ? x : tree[k + i]);
        sk[h] = k, si[h] = i;
        
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &tree[k]);

    bool filled  = (tree[k + B - 2] != INT_MAX);

    insert(tree + k, i, x);

    if (filled) {
        // create a new leaf node
        move(tree + k, tree + n_tree);
        
        int v = tree[k + B / 2 - 1]; // new key to be inserted
        int p = n_tree;              // pointer to the newly created node
        
        n_tree += B;

        for (int h = H - 2; h >= 0; h--) {
            k = sk[h], i = si[h];

            filled = (tree[k + B - 3] != INT_MAX);

            // the node already has a correct key (right one) and a correct pointer (left one) 
            insert(tree + k,     i,     v);
            insert(tree + k + B, i + 1, p);
            
            if (!filled)
                return;

            // create a new internal node
            move(tree + k,     tree + n_tree);     // move keys
            move(tree + k + B, tree + n_tree + B); // move pointers

            v = tree[k + B / 2 - 1];
            tree[k + B / 2 - 1] = INT_MAX;

            p = n_tree;
            n_tree += 2 * B;
        }

        tree[n_tree] = v;

        tree[n_tree + B] = root;
        tree[n_tree + B + 1] = p;

        root = n_tree;
        n_tree += 2 * B;
        H++;
    }
}
