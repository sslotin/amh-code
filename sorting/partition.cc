#include <bits/stdc++.h>

using namespace std;

// return number of elements that are <k and put them in beginning of a
int partition_scalar(int *a, int n, int k) {
    int l = 0, r = n - 1;
    while (l <= r) {
        if (a[l] < k)
            l++;
        else
            swap(a[l], a[r--]);
    }
    return l;
}

int partition_std(int *a, int n, int k) {
    auto it = std::partition(a, a+n, [&](int x){ return x < k; });
    return std::distance(a, it);
}

const int B = 8; // number of elements in a vector

/*
    create pivot vector
    maintain [l, r]
    if (r - l + 1) < 2B
        scalar partition
    else
        load vector starting from l


*/

void partition_vec() {

}


int partition_avx(int *a, int n, int k) {
    return n;
}

void timeit(int (*f)(int*, int, int), int n, int times) {
    clock_t total = 0;
    while (times--) {
        int *a = new int[n];
        for (int i = 0; i < n; i++)
            a[i] = rand();
        int k = a[rand() % n];

        clock_t start = clock();

        f(a, n, k);
        
        total += clock() - start;
    }

    printf("%.6f\n", float(total) / CLOCKS_PER_SEC);
}

int main(int argc, char* argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : (1<<20));
    int times = (argc > 2 ? atoi(argv[2]) : 10);


    timeit(partition_std, n, times);
    timeit(partition_scalar, n, times);
    timeit(partition_avx, n, times);

    return 0;
}
