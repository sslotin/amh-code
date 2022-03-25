#include "btree.hh"
#include <x86intrin.h>

const int B = 32;
typedef __m256i reg;

/*
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
*/

void print(__m256i v) {
    auto t = (unsigned char*) &v;
    for (int i = 0; i < 32; i++)
        std::cout << (t[i] == 255);
    std::cout << std::endl;
}

reg cmp(reg x, int *node) {
    reg y = _mm256_load_si256((reg*) node);
    return _mm256_cmpgt_epi32(x, y);
}

unsigned rank32(reg x, int *node) {
    reg m1 = cmp(x, node);
    reg m2 = cmp(x, node + 8);
    reg m3 = cmp(x, node + 16);
    reg m4 = cmp(x, node + 24);

    //print(m1);
    //print(m2);
    //print(m3);
    //print(m4);

    m1 = _mm256_blend_epi16(m1, m2, 0b01010101);
    //print(m1);

    m3 = _mm256_blend_epi16(m3, m4, 0b01010101);
    //print(m3);

    m1 = _mm256_packs_epi16(m1, m3);
    //print(m1);
    //std::cout << std::endl;

    unsigned mask = _mm256_movemask_epi8(m1);

    return __builtin_popcount(mask);    
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

// move the second half of a node and fill it with infinities
void move(int *from, int *to) {
    const reg infs = _mm256_set1_epi32(INT_MAX);
    for (int i = 0; i < B / 2; i += 8) {
        reg t = _mm256_load_si256((reg*) &from[B / 2 + i]);
        _mm256_store_si256((reg*) &to[i], t);
        _mm256_store_si256((reg*) &from[B / 2 + i], infs); // probably not necessary for pointers
    }
}

void traverse(int k, int h) {
    std::cerr << "n " << k << std::endl << ">";
    for (int i = 0; i < B; i++)
        if (tree[k + i] != INT_MAX)
            std::cerr << " " << tree[k + i];

    if (h > 1) {
        std::cerr << std::endl << ">";
        
        for (int i = 0; i < B; i++)
            if (tree[k + B + i] != INT_MAX)
                std::cerr << " " << tree[k + B + i];

        std::cerr << std::endl;

        for (int i = 0; i < B; i++)
            if (tree[k + B + i] != INT_MAX)
                traverse(tree[k + B + i], h - 1);
    }
}

void insert(int _x) {
    //std::cerr << std::endl << "insert " << _x << std::endl;

    unsigned sk[20], si[20];
    
    unsigned k = root;
    reg x = _mm256_set1_epi32(_x);

    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);
        sk[h] = k, si[h] = i;
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &tree[k]);

    bool filled  = (tree[k + B - 2] != INT_MAX);
    bool updated = (tree[k + i]     == INT_MAX);

    //std::cerr << filled << " " << updated << std::endl;
    
    insert(tree + k, i, _x);

    if (updated) {
        for (int h = H - 2; h >= 0; h--) {
            int idx = sk[h] + si[h]; // todo: unified store
            tree[idx] = (tree[idx] < _x ? _x : tree[idx]);
        }
    }

    /*
    if (updated) {
        // update parent nodes if needed
        for (int h = H - 2; h >= 0; h--) {
            int idx = sk[h] + si[h];
            // unless it is the last element
            if (tree[idx] != INT_MAX)
                tree[idx] = _x;
            if (tree[idx + 1] != INT_MAX)
                break;
            // ^ not needed if root
        }
    }
    */

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
                return; // return?

            // create a new internal node
            move(tree + k,     tree + n_tree);     // move keys
            move(tree + k + B, tree + n_tree + B); // move pointers

            v = tree[k + B / 2 - 1];
            tree[k + B / 2 - 1] = INT_MAX;

            p = n_tree;
            n_tree += 2 * B;
        }

        if (filled) {
            //std::cerr << "tree grows: " << H << " -> " << H + 1 << std::endl;
            tree[n_tree] = v;

            tree[n_tree + B] = root;
            tree[n_tree + B + 1] = p;

            root = n_tree;
            n_tree += 2 * B;
            H++;

            //std::cerr << "the tree:" << std::endl;
            //traverse(root, H);
        }
    }
    
    //std::cerr << "the tree:" << std::endl;
    //traverse(root);
}

int lower_bound(int _x) {
    //std::cerr << std::endl << "lb " << _x << std::endl;
    unsigned k = root;
    reg x = _mm256_set1_epi32(_x);
    
    for (int h = 0; h < H - 1; h++) {
        unsigned i = rank32(x, &tree[k]);
        k = tree[k + B + i];
    }

    unsigned i = rank32(x, &tree[k]);

    return tree[k + i]; // what if next block? maybe we store 31 elements?
}
