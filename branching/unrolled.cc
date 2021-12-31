#include <bits/stdc++.h>

const int N = 1e7, K = 2e8 / N;
int a[N];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    clock_t start = clock();
    volatile int sum = 0;

    for (int k = 0; k < K; k++) {
        /*
        for (int i = 0; i < N; i += 2) {
            sum += (a[i] < 50) * a[i] + (a[i + 1] < 50) * a[i + 1];
            //sum += (a[i] < 50 ? a[i] : 0) + (a[i + 1] < 50 ? a[i + 1] : 0);
        }
        */
        for (int i = 0; i < N; i += 4) {
            /*
            sum += (a[i] < 50) * a[i]
                 + (a[i + 1] < 50) * a[i + 1]
                 + (a[i + 2] < 50) * a[i + 2]
                 + (a[i + 3] < 50) * a[i + 3];
            */
            sum += (a[i] < 50 ? a[i] : 0)
                 + (a[i + 1] < 50 ? a[i + 1] : 0)
                 + (a[i + 2] < 50 ? a[i + 2] : 0)
                 + (a[i + 3] < 50 ? a[i + 3] : 0);
            //sum += (a[i] < 50 ? a[i] : 0) + (a[i + 1] < 50 ? a[i + 1] : 0);
        }
        
    }

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%.4f\n", seconds);
    printf("%d\n", sum);
    
    return 0;
}
