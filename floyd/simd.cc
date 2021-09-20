#include "floyd.hh"

void print(vec x) {
    for (int i = 0; i < 8; i++)
        cout << x[i] << " ";
    cout << endl;
}

vec min(vec x, vec y) {
    return x < y ? x : y;
}

void kernel(vec *a, vec *b, vec c, int n) {
    for (int i = 0; i < n; i++)
        a[i] = min(a[i], b[i] + c);
}

void floyd_main(int *d, int n, int nb) {
    vec *t = (vec*) d;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            vec a = d[i * nb * B + k] + vec{};
            kernel(t + i * nb, t + k * nb, a, nb);
            /*
            cout << k << " " << i << " : " << d[i * nb * B + k] << " : ";
            for (int z = 0; z < 8; z++)
                cout << a[z] << " ";
            cout << endl;
            */
           /*
            for (int j = 0; j < nb; j++) {
                //reg a = _mm256_loadu_si256((reg*) &d[i * n + k]); // bcast
                //reg b = _mm256_loadu_si256((reg*) &d[k * n + j]);
                //reg c = _mm256_loadu_si256((reg*) &d[i * n + j]);
                // vpadd t, a, j  1*FP013
                // vpmin t, t, k  1*FP013
                // vmovd j, t     1*FP2
                //vec x = t[i * nb + j];
                //vec y = a + t[k * nb + j];
                //print(x);
                //print(y);
                //print(x < y ? x : y);
                //t[i * nb + j] = (x < y ? x : y);
                //d[i * n + j] = min(d[i * n + j], d[i * n + k] + d[k * n + j]);
            }*/
        }
    }
}

void printmat(int *t, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << t[i * n + j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    char c;
    cin >> c;
}

void floyd(int *d, int n) {
    const int MAXN = (1 << 11); // ~16MB
    alignas(64) static int t[MAXN * MAXN];

    int nb = (n + B - 1) / B;
    //memset(t, 0, n * nb * B * 4);
    
    for (int i = 0; i < n; i++) {
        memcpy(t + i * nb * B, d + i * n, n * 4);
        for (int j = n; j < nb * B; j++)
            t[i * nb * B + j] = 1e9;
    }

    //printmat(d, n);
    //printmat(t, nb * B);

    floyd_main(t, n, nb);

    for (int i = 0; i < n; i++)
        memcpy(d + i * n, t + i * nb * B, n * 4);

    //printmat(d, n);
}
