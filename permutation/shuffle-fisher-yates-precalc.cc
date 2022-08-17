#include "permutation.hh"

int *random_numbers;

void prepare(int n) {
    random_numbers = new int[n];
    for (int i = 0; i < n; i++)
        random_numbers[i] = rand() % (i + 1);
}

void permute(char *a, int n) {
    for (int i = 0; i < n; i++) {
        int j = random_numbers[i];
        std::swap(a[i], a[j]);
    }
}
