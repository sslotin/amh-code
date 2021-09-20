#include <bits/stdc++.h>
using namespace std;

const int m = 1e9 + 7;

int inverse(int a);

int main() {
    const int n = 1e4, repeat = 1e3;
    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = rand() % (m - 1) + 1;

    volatile int checksum = 0;

    clock_t start = clock();

    for (int i = 0; i < repeat; i++)
        for (int j = 0; j < n; j++)
            checksum += inverse(a[j]);
    
    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%d\n", checksum);
    printf("%.4f s total time\n", seconds);
    printf("%.2f ns per call\n", 1e9 * seconds / n / repeat);
    
    return 0;
}
