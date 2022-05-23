#include <bits/stdc++.h>
using namespace std;

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef __uint128_t u128;

u64 find_factor(u64 n);

int main() {
    int cnt = 0;
    clock_t start = clock();

    u64 n;
    while (cin >> n) {
        u64 d = find_factor(n);
        assert(d != 1 && d != n && n / d * d == n);
        cnt++;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%f\n", cnt / duration);

    return 0;
}
