#include <bits/stdc++.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef D
#define D 8
#endif

int q[N];

const int K = 2e7 / N;

bool is_prime(int n) {
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

int find_prime(int n) {
    int k = n;
    while (!is_prime(k))
        k--;
    return k;
}

int main() {
    int n = find_prime(N);
    //cerr << n << endl;

    for (int i = 0; i < n; i++) {
        q[i] = (2 * i + 1) % n;
        ///cerr << k << endl;
    }
    
    //for (int i = 0; i < n; i++)
    //    cerr << i << " " << q[i] << endl;
    
    clock_t start = clock();

    volatile int s = 0;
    int k = 0;

    for (int t = 0; t < K; t++) {
        for (int i = 0; i < n; i++) {
            //__builtin_prefetch(&q[(2 * k + 1) % n]);
            __builtin_prefetch(&q[(1ll * (1 << D) * k + (1 << D) - 1) % n]);
            k = q[k];
            //cerr << k << endl;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}