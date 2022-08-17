#include <bits/stdc++.h>
#include <x86intrin.h>

void prepare(int n);
void permute(char *a, int n);

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 1024);
    int k = (argc > 2 ? atol(argv[2]) : (1 << 30)) / n;

    prepare(n);

    char* a = (char*) std::aligned_alloc(64, n);

    for (int i = 0; i < n; i++)
        a[i] = rand() % 100;
    
    permute(a, n);

    int checksum = 0;
    for (int i = 0; i < n; i++)
        checksum ^= i * a[i];
    
    printf("%d\n", checksum);

    clock_t start = clock();

    for (int t = 0; t < k; t++) {
        __sync_synchronize();
        permute(a, n);
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.4f seconds\n", duration);
    printf("%.4f B/s\n", 1e-9 * n * k / duration);
    printf("%.4f c/B\n", duration * 2e9 / n / k);

    return 0;
}
