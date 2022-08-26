#include "permutation.hh"

void prepare(int n) {}

void permute(char *a, int n) {
    std::reverse(a, a + n);
}
