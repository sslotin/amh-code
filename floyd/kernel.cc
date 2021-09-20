#include "floyd.hh"

void upd(vec &x, vec y) { 
    x = (x < y ? x : y);
}

/*
// 1 register for top vector
// 1 register for broadcast
// ^ rewritten for sum
// 14 registers remaining for accumulators
// hot loop looks like this:
//   broadcast
//   sum
//   min
// and a read every 14 iterations
void kernel2(int *d, int x, int y, int k0, int n) {
    vec *t = (vec*) d;
    
    vec s[14];
    for (int i = 0; i < 14; i++)
        s[i] = t[(x + i) * n + y];

    for (int k = k0; k < K; k++) {
        vec a = load(d + k * n + y);
        for (int i = 0; i < 14; i++) {
            vec b = d[x * n + k] + vec{};
            s[i] = min(s[i], a + b);
        }
    }

    vec t[14];
    for (int i = 0; i < 14; i++)
        t[i] = load(d + (x + i) * n + y);
}
*/

void kernel(vec *x, vec *b, vec a, int n) {
    for (int i = 0; i < n; i++)
        upd(x[i], a + b[i]);
}

void floyd(int *d, int n) {
    alignas(64) static int t[1 << 22]; // ~16MB
    vec *v = (vec*) t; // shorthand

    int N = (n + B - 1) / B, NB = N * B;
    // todo: pad it with one block in case N is a power of two
    
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
        for (auto [x, y] : order) {
            //cerr << k0 << " " << x << " " << y << endl;
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
        for (int x = 0; x < n; x += K) {
            for (int y = 0; y < n; y += K) {
                if ((x != k0) && (y != k0)) {
                    for (int i = x; i < x + K; i += B) {
                        for (int j = y / B; j < (y + K) / B; j++) {
                            vec s[B]; // will be kept in registers

                            for (int q = 0; q < B; q++)
                                s[q] = v[(i + q) * N + j];

                            // [i:i+B, j:j+B]
                            for (int k = k0; k < k0 + K; k++) {
                                vec b = v[k * N + j];
                                for (int q = 0; q < B; q++) {
                                    vec a = t[(i + q) * NB + k] + vec{};
                                    upd(s[q], a + b);
                                }
                            }

                            for (int q = 0; q < B; q++)
                                v[(i + q) * N + j] = s[q];
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++)
        memcpy(d + i * n, t + i * NB, 4 * n);
}
