#include <bits/stdc++.h>

const int N = 1e3, K = 2e8 / N;
int a[N];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    std::sort(a, a + N);

    clock_t start = clock();
    volatile int sum = 0;

    for (int k = 0; k < K; k++)
        for (int i = 0; i < N; i++)
            if (a[i] < 50)
                sum += a[i];

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%.4f\n", seconds);
    printf("%d\n", sum);
    
    return 0;
}
