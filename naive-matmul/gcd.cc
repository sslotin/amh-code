#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

int main(int argc, char* argv[]) {
    const int n = 1e4, k = 1000;
    int a[n], b[n];

    const int lim = 1e9;

    for (int i = 0; i < n; i++) {
        a[i] = rand() % lim;
        b[i] = rand() % lim;
    }

    volatile int checksum = 0;

    clock_t start = clock();

    for (int i = 0; i < k; i++)
        for (int j = 0; j < n; j++)
            checksum += gcd(a[j], b[j]);
    
    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%d\n", checksum);
    printf("%.4f s total time\n", seconds);
    printf("%.2f ns per call\n", 1e9 * seconds / n / k);
    
    return 0;
}
