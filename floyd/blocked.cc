#include "floyd.hh"

void upd(vec &x, vec y) { 
    x = (x < y ? x : y);
}

void kernel(vec *x, vec *b, vec a, int n) {
    for (int i = 0; i < n; i++)
        upd(x[i], a + b[i]);
}

void floyd(int *d, int n) {
    alignas(64) static int t[1 << 22]; // ~16MB
    vec *v = (vec*) t; // shorthand

    int N = (n + B - 1) / B, NB = N * B;
    
    for (int i = 0; i < n; i++) {
        memcpy(t + i * NB, d + i * n, 4 * n);
        for (int j = n; j < NB; j++)
            t[i * NB + j] = 1e9; // = inf
    }

    const int K = 64;

    // main loop
    for (int k0 = 0; k0 < n; k0 += K) {
        // temporary
        vector< pair<int, int> > order = {{k0, k0}};
        for (int x = 0; x < n; x += K)
            for (int y = 0; y < n; y += K)
                if ((x == k0) ^ (y == k0))
                    order.push_back({x, y});
        for (int x = 0; x < n; x += K)
            for (int y = 0; y < n; y += K)
                if ((x != k0) && (y != k0))
                    order.push_back({x, y});
        for (auto [x, y] : order) {
            // macro-kernel
            for (int k = k0; k < k0 + K; k++) {
                for (int i = x; i < x + K; i++) {
                    vec a = t[i * NB + k] + vec{};
                    kernel(v + i * N + y / B, v + k * N + y / B, a, K / B);
                    //for (int j = y / B; j < y + K / B; j++)
                    //    upd(v[i * N + j], a + v[k * N + j]);
                }
            }
        }
    }

    for (int i = 0; i < n; i++)
        memcpy(d + i * n, t + i * NB, 4 * n);
}
