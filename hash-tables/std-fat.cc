#include "hashmap.hh"

unordered_map<int, int> m;

void setup() {
    m.max_load_factor(0.25);
};

void add(int x, int y) {
    m[x] = y;
}

int get(int x) {
    auto it = m.find(x);
    return (it != m.end() ? it->second : -1);
}
