#include <stdio.h>
#include <time.h>

const int N = 1e6;

int main() {
    clock_t start = clock();

    for (int i = 0; i < N; i++)
        clock();

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2fns\n", 1e9 * duration / N);

    return 0;
}
