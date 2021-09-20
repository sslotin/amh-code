#include "argmin.hh"

int argmin() {
    int k = std::min_element(a, a + n) - a;
    return k;
}
