#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

#ifndef T
#define T 1
#endif

const int K = 1e10 / N;

int a[T][N];//, b[T][N];

void f(int idx) {
    //cout << idx << endl;
    for (int k = 0; k < K; k++) {
        __sync_synchronize();
        for (int i = 0; i < N; i++)
            a[idx][i]++;
    }
        //memcpy(a[idx], b[idx], N);
}

int main() {
    clock_t start = clock();

    #pragma omp parallel for
    for (int i = 0; i < T; i++)
        f(i);
    
    /*
    vector<thread> threads;

    for (int i = 0; i < T; i++) {
        threads.push_back(thread(f, i));
    }
    
    for (auto &th : threads)
        th.join();
    */

    float duration = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%.2f GFlops/s\n", 1e-9 * T * N * K / duration);

    return 0;
}