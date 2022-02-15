#include <bits/stdc++.h>
#include "absl/container/btree_set.h"

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 1 << 20);
    int m = (argc > 2 ? atoi(argv[2]) : 1 << 20);

    int *a = new int[n];
    int *q = new int[m];

    for (int i = 0; i < n; i++)
        a[i] = rand();
    for (int i = 0; i < m; i++)
        q[i] = rand();

    a[0] = RAND_MAX;

    absl::btree_multiset<int> s;
    for (int i = 0; i < n; i++)
        s.insert(a[i]);

    int checksum = 0;
    clock_t start = clock();

    #ifdef LATENCY
    int last = 0;

    for (int i = 0; i < m; i++) {
        last = s.lower_bound(q[i] ^ last);
        checksum ^= last;
    }
    #else
    for (int i = 0; i < m; i++)
        checksum ^= *s.lower_bound(q[i]);
    #endif

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%.2f ns per query\n", 1e9 * seconds / m);
    printf("%d\n", checksum);
    
    return 0;
}

