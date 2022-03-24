// CC=clang++ bazel --batch run absl --copt="-march=native" --copt="-O3"

#include "btree.hh"
#include "absl/container/btree_set.h"

absl::btree_multiset<int> s;

void prepare() {}

void insert(int x) {
    s.insert(x);
}

int lower_bound(int x) {
    return *s.lower_bound(x);
}
