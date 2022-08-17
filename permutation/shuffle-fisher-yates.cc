#include "permutation.hh"

void permute(char *a, int n) {
    for (int i = 0; i < n; i++) {
        int j = random_numbers[i];
        std::swap(a[i], a[j]);
    }
}
