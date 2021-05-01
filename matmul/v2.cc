// Auto-vectorization

#include <algorithm>

void matmul(const float *a, const float *_b, float *c, int n) {
    float *b = new float[n * n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            b[i * n + j] = _b[j * n + i];
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float s = 0;
            for (int k = 0; k < n; k++)
                s += a[i * n + k] * b[j * n + k];
            c[i * n + j] = s;
        }
    }
}
