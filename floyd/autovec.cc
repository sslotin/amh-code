#include "floyd.hh"

void kernel(int * __restrict__ a, int * __restrict__ b, int c, int n) {
    for (int i = 0; i < n; i++)
        a[i] = min(a[i], b[i] + c);
}

void floyd(int *d, int n) {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            kernel(d + i * n, d + k * n, d[i * n + k], n);
            //for (int j = 0; j < n; j++)
                //d[i * n + j] = min(d[i * n + j], d[i * n + k] + d[k * n + j]);
}
