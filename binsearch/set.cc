#include "binsearch.hh"

std::set<int> s;

void prepare(int *a, int n) {
    for (int i = 0; i < n; i++)
        s.insert(a[i]);
}

int lower_bound(int x) {
    return *s.lower_bound(x);
}
