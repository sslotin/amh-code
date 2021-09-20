#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long u64;
typedef __uint128_t u128;

u64 find_factor(u64 n);

int main(int argc, char* argv[]) {
    u64 n = atoll(argv[1]);

    clock_t start = clock();

    u64 d = find_factor(n);
    
    clock_t duration = clock() - start;

    printf("%llu = %llu * %llu\n", n, n / d, d);

    printf("%lu us\n", duration);

    return 0;
}
