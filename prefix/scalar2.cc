#include "prefix.hh"

void prefix(int *a, int n) {
    int s = a[0];
    for (int i = 1; i < n; i++) {
        s += a[i];
        a[i] = s;
    }
}
