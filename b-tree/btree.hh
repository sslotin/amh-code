#include <bits/stdc++.h>
using namespace std;

void prepare();
void add(int x);
int lower_bound(int x);

int main() {
    const int L = 10000, R = 1e7, Q = 1e5;
    const float K = 1.17; // should be ~250 steps

    static int x[R], y[Q];

    x[0] = RAND_MAX;

    for (int i = 1; i < R; i++)
        x[i] = rand();
    
    for (int i = 0; i < Q; i++)
        y[i] = rand();
    
    prepare();

    volatile int checksum = 0;
    
    int size = 0;
    for (int S = L; S < R; S *= K) {
        // benchmark insertions
        clock_t start = clock();
        int cnt = S - size;

        while (size < S)
            add(x[size++]);

        float duration = float(clock() - start) / CLOCKS_PER_SEC;
        printf("%d %.2f", size, 1e9 * duration / cnt);

        // benchmark reads
        start = clock();

        for (int i = 0; i < Q; i++)
            checksum ^= lower_bound(y[i]);

        duration = float(clock() - start) / CLOCKS_PER_SEC;
        printf(" %.2f\n", 1e9 * duration / Q);
    }

    printf("%d\n", checksum);
    
    return 0;
}
