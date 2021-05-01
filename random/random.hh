#include <bits/stdc++.h>
using namespace std;

void seed(int s);
int rng(int l, int r); // [l, r)

int main(int argc, char* argv[]) {
    int n = (int) (argc > 1 ? atof(argv[1]) : 1e6);

    int checksum = 0;

    seed(1);

    for (int i = 0; i < 10; i++)
        printf("%d-", rng(0, 10));

    clock_t start = clock();

    for (int i = 0; i < n; i++)
        checksum += rng(0, 10);

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%d\n", checksum);
    printf("%.4f s total time\n", seconds);
    printf("%.2f ns per value\n", 1e9 * seconds / n);
    
    return 0;
}
