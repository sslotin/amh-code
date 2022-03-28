#include "read.hh"

void prepare() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

int read(int n) {
    int s = 0;
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        s ^= x;
    }
    return s;
}
