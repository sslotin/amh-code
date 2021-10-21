#include <bits/stdc++.h>
using namespace std;

void transpose(int *a, int n, int N) {
    if (n <= 32) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                swap(a[i * N + j], a[j * N + i]);
    } else {
        int k = n / 2;
        transpose(a, k, N);
        transpose(a + k, k, N);
        transpose(a + k * N, k, N);
        transpose(a + k * N + k, k, N);
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                swap(a[i * N + (j + k)], a[(i + k) * N + j]);
        if (n & 1)
            for (int i = 0; i < n - 1; i++)
                swap(a[i * N + n - 1], a[(n - 1) * N + i]);
    }
}

/*
void transpose(int *a, int n, int N) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            swap(a[i * N + j], a[j * N + i]);
}
*/

int main() {
    int n = 7;
    
    int *a = new int[n * n];
    for (int i = 0; i < n * n; i++)
        a[i] = i;

    transpose(a, n, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << a[i * n + j] << " ";
        cout << endl;
    }
    
    return 0;
}
