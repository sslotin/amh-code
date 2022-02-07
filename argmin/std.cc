#include "argmin.hh"

int argmin(int *a, int n) {
    int k = std::min_element(a, a + n) - a;
    return k;
}
