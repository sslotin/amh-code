#include <bits/stdc++.h>
using namespace std;

void setup();
void add(int x, int y); // h[x] := y
int get(int x);         // h[x] if x in h, -1 otherwise

int main() {
    const int base = 1000, lim = 1e7, num_samples = 1e7;
    const float k = 1.1; // should be ~250 steps
    static int x[lim], y[lim], q[num_samples];

    for (int i = 0; i < lim; i++) {
        x[i] = rand();
        y[i] = rand();
    }

    setup();

    volatile int checksum = 0;
    
    int added = 0;
    for (float size = base; size < lim; size *= k) {
        while (added < size) {
            add(x[added], y[added]);
            added++;
        }

        for (int i = 0; i < num_samples; i++)
            q[i] = (rand() & 1 ? rand() : x[i % added]);

        clock_t start = clock();

        for (int i = 0; i < num_samples; i++)
            checksum ^= get(q[i]);

        float seconds = float(clock() - start) / CLOCKS_PER_SEC;

        printf("%d\t%.2f\n", added, 1e9 * seconds / num_samples);
    }

    cerr << checksum << endl;
    
    return 0;
}
