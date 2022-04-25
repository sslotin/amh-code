#include <bits/stdc++.h>
using namespace std;

const int N = 1e7;
const int M = 1e9 + 7;

int inverse(int a);

int main() {
    int *a = new int[N];

    for (int i = 0; i < N; i++)
        a[i] = rand() % (M - 1) + 1;

    volatile int checksum = 0;

    clock_t start = clock();

    for (int i = 0; i < N; i++)
        checksum ^= inverse(a[i]);
    
    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%d\n", checksum);
    printf("%.4f s total time\n", seconds);
    printf("%.2f ns per call\n", 1e9 * seconds / N);
    
    return 0;
}
