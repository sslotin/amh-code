#include "prefix.hh"

void prefix(int *a, int *b, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) {
        s += a[i];
        b[i] = s;
    }
}
