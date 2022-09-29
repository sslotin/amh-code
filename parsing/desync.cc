#include "read.hh"

int read(int n) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int s = 0;
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        s ^= x;
    }
    
    return s;
}
