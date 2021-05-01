#include <iostream>
#include <Eigen/Dense>
 
void matmul(const float *_a, const float *_b, float *_c, int n) {
    Eigen::MatrixXf a(n, n), b(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a(i, j) = _a[i * n + j];
            b(i, j) = _b[i * n + j];
        }
    }
    a = a * b;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            _c[i * n + j] = a(i, j);
        }
    }
}