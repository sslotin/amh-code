#include "permutation.hh"

void prepare(int n) {}

void permute(char *a, int n) {
    for (int i = 0; i < n; i++) {
        int j = rand() % (i + 1);
        std::swap(a[i], a[j]);
    }
}
