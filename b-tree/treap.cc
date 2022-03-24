#include "btree.hh"
#include <cassert>
#include <random>

namespace {
std::minstd_rand0 rnd;

struct node {
    int key, prio;
    node *l, *r;

    node(int key_) : key(key_), prio(rnd()), l(nullptr), r(nullptr) {}
    ~node() {
        delete l;
        delete r;
    }
};

void split(node *v, int key, node **l, node **r) {
    while (v) {
        if (v->key < key) {
            *l = v;
            l = &v->r;
            v = v->r;
        } else {
            *r = v;
            r = &v->l;
            v = v->l;
        }
    }
    *l = *r = nullptr;
    return;
}

void insert(node **v, node *nv) {
    while (true) {
        if (!*v) {
            *v = nv;
            return;
        }
        if (nv->prio > (*v)->prio) {
            // nv is the root
            assert(!nv->l && !nv->r);
            split(*v, nv->key, &nv->l, &nv->r);
            *v = nv;
            return;
        }
        // v is the root
        if (nv->key <= (*v)->key) {
            v = &(*v)->l;
        } else {
            v = &(*v)->r;
        }
    }
}

node *lower_bound(node *v, int x) {
    node *result = nullptr;
    while (v) {
        if (v->key < x) {
            v = v->r;
        } else {
            result = v;
            v = v->l;
        }
    }
    return result;
}

node *root = nullptr;
}

void prepare() {
    delete root;
    root = nullptr;
}

void insert(int x) {
    insert(&root, new node(x));
}

int lower_bound(int x) {
    return lower_bound(root, x)->key;
}
