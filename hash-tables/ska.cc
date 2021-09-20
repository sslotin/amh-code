#include "hashmap.hh"
#include "bytell_hash_map.hpp"

ska::bytell_hash_map<int, int> m;

void setup() {
};

void add(int x, int y) {
    m[x] = y;
}

int get(int x) {
    auto it = m.find(x);
    return (it != m.end() ? it->second : -1);
}
