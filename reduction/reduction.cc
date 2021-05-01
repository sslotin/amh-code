#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

const int N = (1<<12);
const int B = 2; // number of accumulators
alignas(32) int a[N+B];

int normal_sum() {
    int s = 0;
    for (int i = 0; i < N; i++)
        s += a[i];
    return s;
}

int stl_sum() {
    return accumulate(a, a + N, 0);
}


int stl_reduce() {
    return reduce(a, a + N, 0);
}

typedef __m256i reg;

int ilp_autovec_sum() {
    int b[B] = {0};

    for (int i = 0; i < N; i += B)
        for (int j = 0; j < B; j++)
            b[j] += a[i + j];

    int s = 0;

    for (int i = 0; i < B; i++)
        s += b[i];

    return s;
}

int ilp_sum() {
    reg b[B] = { _mm256_setzero_si256() };

    for (int i = 0; i < N; i += 8 * B) {
        for (int j = 0; j < B; j++) {
            reg x = _mm256_load_si256((reg*) &a[i + 8 * j]);
            b[j] = _mm256_add_epi32(b[j], x);
        }
    }

    for (int i = 1; i < B; i++)
        b[0] = _mm256_add_epi32(b[0], b[i]);
    
    int s = 0;

    for (int i = 0; i < 8; i++)
        s += _mm256_extract_epi32(b[0], i);

    return s;
}

typedef int vec __attribute__ (( vector_size(32) ));

int ilp_sum_v2() {
    vec b[B] = {0};
    vec* v = (vec*) a;

    for (int i = 0; i < N/8; i += B)
        for (int j = 0; j < B; j++)
            b[j] += v[i + j];
    
    for (int i = 1; i < B; i++)
        b[0] += b[i];
    
    int s = 0;

    for (int i = 0; i < 8; i++)
        s += b[0][i];

    return s;
}

int dummy() {
    memcpy(a, a, 4 * N);
    return -1;
}

void timeit(int (*f)()) {
    clock_t start = clock();
    volatile int checksum = 0;
    const int cnt = 5e6;

    for (int i = 0; i < cnt; i++) {
        checksum = f();
    }

    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    //cout << seconds << " " << N << " " << cnt << " " << checksum << endl;
    printf("%.6f GFLOPS - %d\n", 1e-9 * N * cnt / seconds, checksum);
}

int main() {
    srand(0);

    for (int i = 0; i < N; i++)
        a[i] = rand() % 100;

    timeit(ilp_sum_v2);
    timeit(normal_sum);
    //timeit(normal_sum);
    //timeit(stl_sum);
    //timeit(ilp_autovec_sum);
    //timeit(stl_reduce);
    //timeit(stl_reduce);
    //timeit(ilp_sum);
    //timeit(ilp_sum_v2);
    timeit(ilp_sum_v2);
    //timeit(dummy);

    return 0;
}
