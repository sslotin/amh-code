#include <bits/stdc++.h>
#include <x86intrin.h>

void prepare();
int read(int n);

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    prepare();

    clock_t start = clock();

    int checksum = read(n);
    printf("%d\n", checksum);

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f %.2f\n", duration, 1e9 * duration / n);

    return 0;    
}
