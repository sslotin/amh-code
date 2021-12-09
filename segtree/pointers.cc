#include "prefix.hh"

struct segtree {
    int lb, rb;
    int s = 0;
    segtree *l = 0, *r = 0;

    segtree(int lb, int rb) : lb(lb), rb(rb) {
        if (lb + 1 < rb) {
            int t = (lb + rb) / 2;
            l = new segtree(lb, t);
            r = new segtree(t, rb);
        }
    }
    
    void add(int k, int x) {
        s += x;
        if (l) {
            if (k < l->rb)
                l->add(k, x);
            else
                r->add(k, x);
        }
    }
    
    int sum(int k) { // [0, k)
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
    return root.sum(k + 1);
}
