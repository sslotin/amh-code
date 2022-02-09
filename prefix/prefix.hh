#include <bits/stdc++.h>

void prefix(int *a, int *b, int n);

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 4096);

    auto a = (int*) std::aligned_alloc(64, 4 * n);
    auto b = (int*) std::aligned_alloc(64, 4 * n);

    for (int i = 0; i < n; i++)
        a[i] = rand() % 100;

    int k = 5e9 / n;

    clock_t start = clock();

    for (int i = 0; i < k; i++) {
        __sync_synchronize();
        prefix(a, b, n);
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f GFLOPS\n", 1e-9 * n * k / duration);
    
    int checksum = 0;

    for (int i = 0; i < n; i++)
        checksum ^= b[i] * i;

    printf("%d\n", checksum);

    for (int i = 0; i < 10; i++)
        printf("%d %d\n", a[i], b[i]);

    return 0;
}
