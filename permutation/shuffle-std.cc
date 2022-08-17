#include "permutation.hh"

void prepare(int n) {}

void permute(char *a, int n) {
    std::random_shuffle(a, a + n);
}
