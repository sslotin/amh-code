#include "prefix.hh"

void prefix(int *a, int *b, int n) {
    b[0] = a[0];
    for (int i = 1; i < n; i++)
        b[i] = b[i - 1] +  a[i];
}
