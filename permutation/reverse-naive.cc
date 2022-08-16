#include "permutation.hh"

void permute(char *a, int n) {
    for (int i = 0; i < n / 2; i++)
        std::swap(a[i], a[n - i - 1]);
}
