#include <bits/stdc++.h>

int f1(int n) {
    if (n == 1)
        return 1;
    return f1(n - 1) * n;
}

int f2(int n) {
    int r = 1;
    while (n > 1) {
        r *= n;
        n--;
    }
    return r;
}

int f3(int n, int acc = 1) {
    return (n == 1 ? acc : f3(n - 1, acc * n));
}

int main() {
    const int n = 1e7;

    volatile int checksum = 0;

    clock_t start = clock();

    for (int t = 0; t < n; t++)
        for (int i = 1; i < 10; i++)
            checksum += f3(i);

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%d\n", checksum);
    printf("%.4f s total time\n", seconds);
    //printf("%.2f ns per call\n", 1e9 * seconds / n / k);

    return 0;
}