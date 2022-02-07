#include "argmin.hh"

int argmin(int *a, int n) {
    int k = 0;
    for (int i = 0; i < n; i++)
        if (__builtin_expect_with_probability(a[i] < a[k], true, 0.5))
            k = i;
    return k;
}
