#include <bits/stdc++.h>
using namespace std;

void add(int k, int x); // 0-based indexation
int sum(int k); // sum of elements indexed [0, k]

const int n = (1<<16), q = 5e7;
int aq[q], xq[q], bq[q];

int main() {
    for (int i = 0; i < q; i++) {
        aq[i] = rand() % n;
        bq[i] = rand() % (n - 1);
        xq[i] = rand() % 10;
    }

    // warmup run
    for (int i = 0; i < q; i++)
        add(aq[i], xq[i]);

    clock_t start = clock();

    for (int i = 0; i < q; i++)
        add(aq[i], xq[i]);

    printf("%.4f\n", 1e9 * (clock() - start) / CLOCKS_PER_SEC / q);

    start = clock();
    int checksum = 0;

    for (int i = 0; i < q; i++)
        checksum ^= sum(bq[i]);

    printf("%.4f\n", 1e9 * (clock() - start) / CLOCKS_PER_SEC / q);
    printf("%d\n", checksum);
    
    return 0;
}
