#include "permutation.hh"

void prepare() {}

void permute(char *a, int n) {
    std::reverse(a, a + n);
}
