#include <bits/stdc++.h>
#include <sys/mman.h>
#include <unistd.h>

const int m = 1e5;
const int limit = UINT_MAX / m + 1;

constexpr int crc(int d, int i) {
    return (d + int('0')) * i;
}

struct Precalc {
    int high[limit];
    int low[6][m];
    int digits[limit];

    constexpr Precalc() : high{}, low{}, digits{} {
        for (int x = 0; x < limit; x++) {
            for (int y = x; y != 0; y /= 10)
                digits[x]++;
            for (int y = x, d = digits[x] - 1; y != 0; y /= 10, d--)
                high[x] += crc(y % 10, d);
        }
        for (int d = 0; d < 6; d++) {
            for (int x = 0; x < m; x++) {
                if (d == 0) {
                    int t = 0;
                    for (int y = x; y != 0; y /= 10)
                        t++;
                    for (int y = x; y != 0; y /= 10, t--)
                        low[d][x] += crc(y % 10, t - 1);
                } else {
                    int t = d + 5;
                    for (int i = 0, y = x; i < 5; i++, t--, y /= 10)
                        low[d][x] += crc(y % 10, t - 1);

                }
            }
        }
    }
};

constexpr Precalc P;

int main() {
    int fsize = lseek(0, 0, SEEK_END);
    auto input = (uint32_t*) mmap(0, fsize, PROT_READ, MAP_PRIVATE, 0, 0);

    uint64_t res = 0;

    for (int i = 0; i < fsize / 4; i++) {
        uint32_t x = input[i];
        uint32_t hi = x / m;
        uint32_t lo = x - hi * m;
        int d = P.digits[hi];
        res += P.high[hi] + P.low[d][lo];

        /*std::cout << x << " "
                  << hi << " "
                  << lo << " "
                  << d << " "
                  << P.high[hi] << " "
                  << P.low[d][lo] << std::endl; */
    }

    std::cout << res << std::endl;

    return 0;
}
