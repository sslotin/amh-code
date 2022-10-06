#include <iostream>

int main() {
    uint64_t res = 0;
    while (std::cin) {
        uint32_t n = 0;
        std::cin.read((char *) &n, sizeof(uint32_t));
        auto s = std::to_string(n);
        for (int i = 0; i < s.size(); ++i) {
            res += s[i] * i;
        }
    }

    std::cout << res << std::endl;
    return 0;
}
