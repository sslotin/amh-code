#include "prefix.hh"

void prefix(int *a, int n) {
    std::partial_sum(a, a + n, a);
}
