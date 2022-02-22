#include "prefix.hh"

struct segtree {
    int lb, rb;
    int s = 0;
    segtree *l = nullptr, *r = nullptr;

    segtree(int lb, int rb) : lb(lb), rb(rb) {
        if (lb + 1 < rb) {
            int t = (lb + rb) / 2;
            l = new segtree(lb, t);
            r = new segtree(t, rb);
        }
    }
    
    void add(int k, int x) {
        s += x;
        if (l != nullptr) {
            if (k < l->rb)
                l->add(k, x);
            else
                r->add(k, x);
        }
    }
    
    int sum(int k) {
        if (rb <= k)
            return s;
        if (lb >= k)
            return 0;
        return l->sum(k) + r->sum(k);
    }
};

segtree root(0, N);

void add(int k, int x) {
    root.add(k, x);
}

int sum(int k) {
    return root.sum(k);
}
