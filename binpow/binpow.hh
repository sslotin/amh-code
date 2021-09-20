#include <bits/stdc++.h>
using namespace std;

const int m = 1e9 + 7;

int binpow(int a, int b);

int main(int argc, char* argv[]) {
    const int n = 1e4, repeat = 500;
    int a[n], b[n];

    for (int i = 0; i < n; i++) {
        a[i] = rand() % m;
        b[i] = rand() % m;
    }

    volatile int checksum = 0;

    clock_t start = clock();

    for (int i = 0; i < repeat; i++)
        for (int j = 0; j < n; j++)
            checksum += binpow(a[j], b[j]);
    
    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%d\n", checksum);
    printf("%.4f s total time\n", seconds);
    printf("%.2f ns per call\n", 1e9 * seconds / n / repeat);
    
    return 0;
}
