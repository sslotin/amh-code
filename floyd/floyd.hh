#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

const int B = 8;

typedef __m256i reg;
typedef int vec __attribute__ (( vector_size(4 * B) ));

void floyd(int *d, int n);

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 256);
    float testing_duration = (argc > 2 ? stof(argv[2]) : 1);

    int *d = new int[n * n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            d[i * n + j] = (i == j ? 0 : rand() % 1000000);
    
    floyd(d, n);
    int checksum = 0;
    for (int i = 0; i < n * n; i++)
        checksum ^= d[i];
    printf("%d\n", checksum);

    clock_t start = clock();

    int cnt = 0;
    while (clock() - start < testing_duration * CLOCKS_PER_SEC) {
        floyd(d, n);
        cnt++;
    }

    float avg = float(clock() - start) / CLOCKS_PER_SEC / cnt;

    printf("%.6f x %d\n", avg, cnt);
    printf("%.2f GFLOPS\n", 1e-9 * n * n * n / avg);

    return 0;
}
