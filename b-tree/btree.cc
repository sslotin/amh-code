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

alignas(64) int tree[R];
int n_tree = B; // 31 (+ 1) + 32 for internal nodes and 31 for data nodes

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
        tree[i] = INT_MAX;
}

void insert(int *node, int i, int x) {
    for (int j = B - 8; j >= 0; j -= 8) {
        reg t = _mm256_load_si256((reg*) &node[j]);
        reg mask = _mm256_load_si256((reg*) &P.mask[i][j]);
        _mm256_maskstore_epi32(&node[j + 1], mask, t);
    }
    node[i] = x;
}

/*
// also moves pointers
void insert2(int *node, int i, int x) {
    for (int j = B - 8; j >= 0; j -= 8) {
        reg mask = _mm256_load_si256((reg*) &P.mask[i][j]);

        reg t1 = _mm256_load_si256((reg*) &node[j]);
        _mm256_maskstore_epi32(&node[j + 1], mask, t1);

        // wrong?
        reg t2 = _mm256_load_si256((reg*) &node[B + j]);
        _mm256_maskstore_epi32(&node[B + j + 1], mask, t2);
    }
    node[i] = x;
}
*/

// move the second half of a node and fill it with infinities
void move(int *from, int *to) {
    for (int j = B / 2; j < B; j += 8) {
        const reg infs = _mm256_set1_epi32(INT_MAX);
        reg t = _mm256_load_si256((reg*) from + j);
        _mm256_store_si256((reg*) to + j, t);
        _mm256_store_si256((reg*) from + j, infs); // probably not necessary for pointers
    }
}

void traverse(int k) {
    std::cerr << "n " << k << std::endl << ">";
    for (int i = 0; i < B; i++)
        if (tree[k + i] != INT_MAX)
            std::cerr << " " << tree[k + i];

    std::cerr << std::endl << ">";
    
    for (int i = 0; i < B; i++)
        if (tree[k + B + i] != INT_MAX)
            std::cerr << " " << tree[k + B + i];

    std::cerr << std::endl;

    for (int i = 0; i < B; i++)
        if (tree[k + B + i] != INT_MAX)
            traverse(tree[k + B + i]);
}

void insert(int _x) {
    std::cerr << std::endl << "insert " << _x << std::endl;

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

    unsigned i = rank32(x, &tree[k]);

    bool filled  = (tree[k + B - 2] != INT_MAX);
    bool updated = (tree[k + i]     == INT_MAX);

    std::cerr << filled << " " << updated << std::endl;
    
    insert(tree + k, i, _x);

    if (filled) {
        // create a new leaf node
        move(tree + k, tree + n_tree);
        n_tree += B;
        
        unsigned l = tree[k + B / 2 - 1];

        while (ss) {
            k = sk[ss];
            i = si[ss];
            ss--;

            filled  = (tree[k + B - 2] != INT_MAX);
            updated = (tree[k + i]     == INT_MAX);

            insert(tree + k,     i,     _x);
            insert(tree + k + B, i + 1, n_tree);
            
            if (!filled)
                break;

            // create a new internal node
            move(tree + k, tree + n_tree); // keys
            move(tree + k + B, tree + n_tree + B); // pointers
            n_tree += 2 * B;
        }
    }

    // if we've split the root
    if (ss == 0 && filled) {
        tree[n_tree + B] = root;
        tree[n_tree + B + 1] = n_tree - 2 * B;
        
        tree[n_tree] = tree[root + B / 2 - 1]; // last element of the current root
        tree[n_tree + 1] = tree[n_tree - 2 * B + B / 2 - 1]; // last element of the newly created node

        root = n_tree;
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

    std::cerr << "the tree:" << std::endl;
    traverse(root);
}

int lower_bound(int _x) {
    std::cerr << std::endl << "lb " << _x << std::endl;
    unsigned k = root;
    reg x = _mm256_set1_epi32(_x - 1);
    
    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &tree[k]);

    return tree[k + i]; // what if next block? maybe we store 31 elements?
}
