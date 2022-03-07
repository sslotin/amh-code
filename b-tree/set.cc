#include "btree.hh"

std::multiset<int> s;

void prepare() {}

void add(int x) {
    s.insert(x);
}

int lower_bound(int x) {
    return *s.lower_bound(x);
}
