#include "permutation.hh"

void prepare(int n) {}

void permute(char *a, int n) {
    n /= 4;
    int *b = (int*) a;
    for (int i = 0; i < n / 2; i++)
        std::swap(b[i], b[n - i - 1]);
}
