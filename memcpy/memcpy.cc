#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

const int n = (1<<13), k = 1e10 / n, t = 1;
alignas(32) char a[t][n], b[t][n];

const int B = 8; // number of elements in a vector
typedef char reg __attribute__ (( vector_size(32) ));

void f(int idx) {
    for (int i = 0; i < k; i++) {
        //memset(a[idx], 0, n);
        //memcpy(b[idx], a[idx], n);
        //for (int j = 0; j < n; j++)
        //    a[idx][j] += b[idx][j];
        //    a[idx][j] = b[idx][j];
        
        reg* x = (reg*) &a[idx][0];
        reg* y = (reg*) &b[idx][0];

        for (int j = 0; j < n/32; j++) {
            //x[j] = x[j] + y[j];
            __m256i xv = _mm256_load_si256((__m256i*) &x[j]);
            __m256i yv = _mm256_load_si256((__m256i*) &y[j]);
            __m256i z = _mm256_add_epi32(xv, yv);
            _mm256_store_si256((__m256i*) &x[j], z);
        }

        
        /*
        const int B = 8;
        typedef __m256i reg;
        const reg z = _mm256_setzero_si256();
        
        reg r;

        for (int j = 0; j < n; j += 32) {
            for (int b = 0; b < B; b++) {
                reg x = _mm256_load_si256((reg*) &a[idx][j + 32 * b]);
                r = _mm256_add_epi8(r, x);
            }
            //reg y = _mm256_load_si256((reg*) &b[idx][j]);
            //reg z = _mm256_add_epi8(x, y);
            //_mm256_store_si256((reg*) &a[idx][j], z);
        }*/
    }
}

int main() {
    clock_t start = clock();

    vector<thread> threads;

    for (int i = 0; i < t; i++)
        threads.push_back(thread(f, i));
    
    for (auto &th : threads)
        th.join();

    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    cout << n << " " << k << " " << t << " " << duration << endl;
    printf("%.2f GB/s\n", 1e-9 * n * k * t / duration);

    return 0;
}