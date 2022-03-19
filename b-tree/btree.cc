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

const int R = 1e8; // reserve

alignas(64) int tree[R], data[R];
int n_tree = 0, n_data = B; // 31 (+ 1) + 32 and 31

int root = 0;

struct Precalc {
    alignas(64) int mask[B][B];

    constexpr Precalc() : mask{} {
        for (int i = 0; i < B; i++)
            for (int j = i; j < B - 1; j++)
                mask[i][j] = -1;
    }
};

constexpr Precalc P;

void prepare() {
    for (int i = 0; i < R; i++)
        tree[i] = data[i] = INT_MAX;
}

void insert(int *node, int i, int x) {
    for (int j = B - 8; j >= 0; j -= 8) {
        reg t = _mm256_load_si256((reg*) &node[j]);
        reg mask = _mm256_load_si256((reg*) &P.mask[i][j]);
        _mm256_maskstore_epi32(&node[j + 1], mask, t);
    }
    node[i] = x;
}

// move the second half of a node
void move(int *from, int *to) {
    for (int j = B / 2; j < B; j += 8) {
        const reg infs = _mm256_set1_epi32(INT_MAX);
        reg t = _mm256_load_si256((reg*) from + j);
        _mm256_store_si256((reg*) to + j, t);
        _mm256_store_si256((reg*) from + j, infs);
    }
}

void insert(int _x) {
    unsigned sk[20], si[20], ss = 0;
    
    unsigned k = root;
    reg x = _mm256_set1_epi32(_x - 1);

    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);
        sk[ss] = k;
        si[ss] = i;
        ss++;
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &data[k]);

    bool filled  = (data[k + B - 2] != INT_MAX);
    bool updated = (data[k + i]     == INT_MAX);
    
    insert(data + k, i, _x);

    if (filled) {
        // create a new leaf node
        move(data + k, data + n_data);
        n_data += B;
        
        unsigned l = data[k + B / 2 - 1];

        while (ss) {
            k = sk[ss];
            i = si[ss];
            ss--;

            filled  = (tree[k + B - 2] != INT_MAX);
            updated = (tree[k + i]     == INT_MAX);

            tree[k + i] = _x;
            insert(tree + k, i, l);
            // todo: pointers too

            if (!filled)
                break;

            // create a new internal node
            move(tree + k, tree + n_tree); // keys
            move(tree + k + B, tree + n_tree + B); // pointers
            n_tree += 2 * B;
        }
    }

    if (ss == 0 && filled) {
        // todo: grow the tree upwards
        root = n_tree;
        tree[n_tree] = // last element of current root;
        n_tree += 2 * B;
        H++;
    }
    
    while (ss && updated) {
        k = sk[ss];
        i = si[ss];
        ss--;

        tree[k + i] = _x;

        updated = (tree[k + i] == INT_MAX);
    }
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
