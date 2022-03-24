#include <bits/stdc++.h>

void prepare();
void insert(int x);
int lower_bound(int x);

int main() {
    const int L = 10000, R = 1e7, Q = 5e5;
    const float K = 1.17; // should be ~250 steps

    static int x[R], y[Q];

    const int LIM = (1 << 30);

    x[0] = LIM;

    for (int i = 1; i < R; i++)
        x[i] = rand() % LIM;
    
    prepare();

    volatile int checksum = 0;
    
    /*
    int size = 0;
    for (int S = 1; S <= L; S += 10) {
        // benchmark insertions
        for (int i = 0; i < 10; i++)
            insert(x[size++]);

        for (int i = 0; i < Q; i++)
            y[i] = rand() % LIM;

        // benchmark reads
        clock_t start = clock();

        for (int i = 0; i < Q; i++)
            checksum ^= lower_bound(y[i]);

        float duration = float(clock() - start) / CLOCKS_PER_SEC;
        printf("%d %.2f\n", size, 1e9 * duration / Q);
    }
    */

    int size = 0;
    for (int S = L; S < R; S *= K) {
        // benchmark insertions
        clock_t start = clock();
        int cnt = S - size;

        while (size < S)
            insert(x[size++]);

        float duration = float(clock() - start) / CLOCKS_PER_SEC;
        printf("%d %.2f", size, 1e9 * duration / cnt);

        for (int i = 0; i < Q; i++)
            y[i] = rand() % LIM;

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
