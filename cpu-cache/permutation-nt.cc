#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

#ifndef N
#define N (1<<20)
#endif

int p[N+4], q[N+4];

const int K = (1<<25) / N;

alignas(32) int buff[4];

int load(int *a) {
    int idx = (uint64_t(a) % 16) / 4;
    __m128i *ptr = (__m128i*) (uint64_t(a) / 16 * 16);
    __m128i x = _mm_stream_load_si128(ptr);
    _mm_store_si128((__m128i*) buff, x);
    //cerr << buff[idx] << endl;
    //cerr << a << " " << ptr << " " << idx << endl;
    return buff[idx];
}

int garbage[1<<20];

int main() {
    iota(p, p + N, 0);
    random_shuffle(p, p + N);

    int k = p[N - 1];

    for (int i = 0; i < N; i++) {
        q[k] = p[i];
        k = p[i];
    }

    
    //for (int i = 0; i < (1<<20); i++)
    //    s += garbage[i];
    
    //memset(garbage, 0, sizeof garbage);

    clock_t start = clock();

    volatile int s = 0;

    for (int t = 0; t < K; t++) {
        #pragma GCC unroll 8
        for (int i = 0; i < N; i++) {
            //__builtin_prefetch(&a[i + 10 * D]);
            k = load(&q[k]);
            //cerr << k << endl;
        }
        s ^= k;
    }

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    printf("%.2f ns\n", 1e9 * duration / (N * K));

    return 0;
}
