#include <bits/stdc++.h>

const int mod = 1e9 + 7;

int gcd(int a, int b) {
    int cnt = 0;
    while (b > 0) {
        a %= b;
        std::swap(a, b);
        cnt++;
    }
    return cnt;
}

int main() {
    int worst = 0;

    for (int i = 1; i < mod; i++) {
        int cnt = gcd(i, mod);
        if (cnt > worst) {
            worst = cnt;
            printf("%d %d\n", i, worst);
        }
    }

    return 0;
}
