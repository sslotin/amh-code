#pragma GCC optimize("O3") 

#include <bits/stdc++.h>
using namespace std;

int mygcd(int a, int b) {
    if (!a || !b) return a | b;

    char az = __builtin_ctz(a);
    char bz = __builtin_ctz(b);
    char shift = min(az, bz);
    a >>= az;
    
    while (true) {
        b >>= bz;
        int dba = b - a;
        int dab = a - b;
        bz = __builtin_ctz(dba);
        if (dba == 0) break;
        a = min(a, b);
        b = max(dba, dab);
    }
    
    return a << shift;
}

const int n = 1e4, k = 1000;
int a[n], b[n];

void timeit(int (*f)(int, int)) {
    clock_t start = clock();

    volatile int checksum = 0;


    for (int i = 0; i < k; i++)
        for (int j = 0; j < n; j++)
            checksum += f(a[j], b[j]);
    
    float seconds = float(clock() - start) / CLOCKS_PER_SEC;

    printf("%.2f ns per call\n", 1e9 * seconds / n / k);
    
    cout << double(clock() - start) / CLOCKS_PER_SEC << endl;
}


int main() {
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 1000000000;
        b[i] = rand() % 1000000000;
    }

    timeit(gcd);
    timeit(mygcd);

    return 0;
}
