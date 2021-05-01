#include <bits/stdc++.h>
using namespace std;

const int n = 1e6;
int a[n];

void setup() {
    for (int i = 0; i < n; i++)
        a[i] = rand();
    sort(a, a + n);
}

void query() {
    int checksum = 0;
    for (int i = 0; i < n; i++) {
        int idx = lower_bound(a, a + n, rand()) - a;
        checksum += idx;
    }
    cout << checksum << endl;
}

int main() {
    setup();
    query();

    return 0;
}
