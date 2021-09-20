#include "floyd.hh"

void floyd(int *d, int n) {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                d[i * n + j] = min(d[i * n + j], d[i * n + k] + d[k * n + j]);
}
