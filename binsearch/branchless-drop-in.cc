#include "binsearch.hh"
//#include <cstring>

int n;
int *t;

void prepare(int *a, int _n) {
    t = new int[n = _n];
    memcpy(t, a, 4 * n);
}

int lower_bound(int x) {
    int l = 0, r = n - 1;
    while (l < r) {
        int m = (l + r) / 2;
        bool cond = __builtin_unpredictable(t[m] >= x);
        r = (cond ? m : r);
        l = (cond ? l : m + 1);
        /*
        asm("cmp %0, %1;"
            "mov  %1, %%fs;"
            "mov  %%fs:(%2), %0\n\t"
            "pop  %%fs"
            : "=r"(ret) : "g"(sel), "r"(off) );
        */
    }
    return t[l];
}
