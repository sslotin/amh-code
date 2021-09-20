#include <bits/stdc++.h>
using namespace std;

typedef complex<double> ftype;

void fft(ftype *a, int n);

int main() {
    const int n = (1<<20);
    alignas(64) static ftype a[n];

    for (int i = 0; i < n; i++)
        a[i] = i;

    fft(a, n);

    for (int i = 0; i < 8; i++)
        cout << a[i] << endl;

    cout << endl;

    //char c;
    //cin >> c;

    clock_t start = clock();
    int cnt = 0;

    while (clock() - start < CLOCKS_PER_SEC) {
        fft(a, n);
        cnt++;
    }

    float avg = float(clock() - start) / CLOCKS_PER_SEC / cnt;

    printf("%.8f x %d\n", avg, cnt);
    printf("%.2f ns / element\n", 1e9 * avg / n);

    return 0;
}
