#include <bits/stdc++.h>
#include <x86intrin.h>

void prepare();
int read(int n);

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 1e7);
    prepare();

    clock_t start = clock();

    int checksum = read(n);
    printf("%d\n", checksum);

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.4f %.4f %.4f\n", duration, 1e9 * duration / n, 2e9 * duration / (n * 8.9));

    return 0;    
}
