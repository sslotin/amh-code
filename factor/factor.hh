#include <bits/stdc++.h>
using namespace std;

typedef __uint16_t u16;
typedef __uint32_t u32;
typedef __uint64_t u64;
typedef __uint128_t u128;

u64 find_factor(u64 n);

int main() {
    int cnt = 0, err = 0;
    clock_t start = clock();

    u64 n;
    while (cin >> n) {
        u64 d = find_factor(n);
        err += !(d != 1 && d != n && n / d * d == n);
        cnt++;
    }

    //assert(cnt > err * 100);

    float duration = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%f %d/%d\n", cnt / duration, err, cnt);

    return 0;
}
