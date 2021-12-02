#pragma GCC optimize("O3")
#pragma GCC target("avx2")
 
#include <x86intrin.h>
#include <bits/stdc++.h>
 
using namespace std;
 
const int n = (1<<22), m = (1<<23);
int a[n], q[m], results[m];
alignas(64) int b[n+1], c[n];
 
const int block_size = 16;
const int B = block_size;
const int INF = numeric_limits<int>::max();
 
int eytzinger(int i = 0, int k = 1) {
    if (k <= n) {
        i = eytzinger(i, 2 * k);
        b[k] = a[i++];
        i = eytzinger(i, 2 * k + 1);
    }
    return i;
}
 
int search(int x) {
    int k = 1;
    while (k <= n) {
        __builtin_prefetch(b + k * block_size);
        k = 2 * k + (b[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return k;
}
 
const int nblocks = (n + B - 1) / B;
alignas(64) int btree[nblocks][B];
 
int go(int k, int i) { return k * (B + 1) + i + 1; }
 
void build(int k = 0) {
    static int t = 0;
    if (k < nblocks) {
        for (int i = 0; i < B; i++) {
            build(go(k, i));
            btree[k][i] = (t < n ? a[t++] : INF);
        }
        build(go(k, B));
    }
}
 
int bsearch(int x) {
    int k = 0, res = INF;
    start:
    while (k < nblocks) {
        for (int i = 0; i < B; i++) {
            if (btree[k][i] >= x) {
                res = btree[k][i];
                k = go(k, i);
                goto start;
            }
        }
        k = go(k, B);
    }
    return res;
}
 
typedef __m256i reg;
 
int cmp(reg x_vec, int* y_ptr) {
    reg y_vec = _mm256_load_si256((reg*) y_ptr);
    reg mask = _mm256_cmpgt_epi32(x_vec, y_vec);
    return _mm256_movemask_ps((__m256) mask);
}
 
int bsearch_avx2(int x) {
    int k = 0, res = INF;
    reg x_vec = _mm256_set1_epi32(x);
    while (k < nblocks) {
        int mask = ~(
            cmp(x_vec, &btree[k][0]) +
            (cmp(x_vec, &btree[k][8]) << 8)
        );
        int i = __builtin_ffs(mask) - 1;
        if (i < B)
            res = btree[k][i];
        k = go(k, i);
    }
    return res;
}
 
template<typename lambda>
double timeit(string name, lambda f) {
    cout << "> " << name << endl;
    clock_t start = clock();
    f();
    double t = double(clock() - start) / CLOCKS_PER_SEC;
    cout << "time: " << t << endl;
    return t;
}
 
int main() {
    for (int i = 0; i < n; i++)
        a[i] = (rand() << 16) + rand();
    for (int i = 0; i < m; i++)
        q[i] = (rand() << 16) + rand();
    a[0] = INF;
 
    sort(a, a+n);
    eytzinger();
    build();
 
    double t1 = timeit("std::lower_bound", [&](){
        for (int i = 0; i < m; i++)
            results[i] = *lower_bound(a, a + n, q[i]);
    });
 
    timeit("eytzinger", [&](){
        for (int i = 0; i < m; i++) {
            assert(results[i] == b[search(q[i])]);
        }
    });
 
    timeit("btree", [&](){
        for (int i = 0; i < m; i++) {
            assert(results[i] == bsearch(q[i]));
        }
    });
 
    double t2 = timeit("btree-avx2", [&](){
        for (int i = 0; i < m; i++) {
            assert(results[i] == bsearch_avx2(q[i]));
        }
    });
    
    cout << "spedup: " << t1 / t2 << endl;
 
    return 0;
}