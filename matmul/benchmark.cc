#include <bits/stdc++.h>
#include "matmul.hh"

using namespace std;

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 256);
    float testing_duration = (argc > 2 ? stof(argv[2]) : 1);

    float *a = new float[n * n];
    float *b = new float[n * n];
    float *c = new float[n * n];

    memset(c, 0, 4 * n * n);

    clock_t start = clock();
    int cnt = 0;
    while (clock() - start < testing_duration * CLOCKS_PER_SEC) {
        matmul(a, b, c, n);
        cnt++;
    }

    printf("%.6f x %d\n", float(clock() - start) / CLOCKS_PER_SEC / cnt, cnt);

    return 0;
}
