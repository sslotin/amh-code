#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef Q
#define Q (1<<20)
#endif

void add(int k, int x); // 0-based indexation
int sum(int k); // sum of elements indexed [0, k)

int aq[Q], xq[Q], bq[Q];

int main() {
    for (int i = 0; i < Q; i++) {
        aq[i] = rand() % N;
        bq[i] = rand() % (N - 1) + 1;
        xq[i] = rand() % 10;
    }

    // warm-up run
    // for (int i = 0; i < q; i++)
    //     add(aq[i], xq[i]);

    clock_t start = clock();

    for (int i = 0; i < Q; i++)
        add(aq[i], xq[i]);

    printf("%.2f\n", 1e9 * (clock() - start) / CLOCKS_PER_SEC / Q);

    start = clock();
    int checksum = 0;

    for (int i = 0; i < Q; i++)
        checksum ^= sum(bq[i]);

    printf("%.2f\n", 1e9 * (clock() - start) / CLOCKS_PER_SEC / Q);
    printf("%d\n", checksum);
    
    return 0;
}
