// bazel run absl --copt="-march=native" --copt="-O3" --copt="-funroll-loops"

#include "btree.hh"
#include "absl/container/btree_set.h"

absl::btree_multiset<int> s;

void prepare() {}

void add(int x) {
    s.insert(x);
}

int lower_bound(int x) {
    return *s.lower_bound(x);
}
