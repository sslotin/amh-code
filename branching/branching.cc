#include <bits/stdc++.h>

#ifndef N
#define N 1'000'000
#endif

#ifndef T
#define T 1e8
#endif

#ifndef P
#define P 50
#endif

const int K = T / N;
int a[N];

int main() {
    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    #ifdef SORT
    std::sort(a, a + N);
    #endif

    clock_t start = clock();
    volatile int s = 0;

    for (int k = 0; k < K; k++)
        for (int i = 0; i < N; i++)
            #ifdef CMOV
            s += (a[i] < P ? a[i] : 0);
            #else
            //if (__builtin_expect(a[i] < P, false))// [[unlikely]]
            if (a[i] < P)
                s += a[i];
            #endif

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;
    float per_element = 1e9 * seconds / K / N;

    printf("%.4f %.4f %.4f\n", seconds, per_element, 2 * per_element);
    printf("%d\n", s);
    
    return 0;
}
