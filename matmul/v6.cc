// Micro-kernel

#include <bits/stdc++.h>
#include <x86intrin.h>

const int B = 8; // number of elements in a vector
typedef float vector __attribute__ (( vector_size(4 * B) ));

float* alloc(int n) {
    float* ptr = (float*) std::aligned_alloc(64, 4 * n);
    memset(ptr, 0, 4 * n); 
    return ptr;
}

// c: 6 x 16
// a: 6 x k
// b: k x 16
// c[x:x+6][y:y+16] += a[x:x+6][l:r] * b[l:r][y:y+16]

inline int idx_a(int i, int j, int n) {
    //return i * n + j;
    return i / 6 * 6 * n + j * 6 + i % 6;
}

inline int idx_b(int i, int j, int n) {
    //return i * n + j;
    return j / 16 * 16 * n + i * 16 + j % 16;
}

void kernel(const float *a, const vector *b, vector __restrict__ *c, int x, int y, int l, int r, int n) {
    vector t[6][2] = {0};

    for (int k = l; k < r; k++) {
        __builtin_prefetch(&b[n / 8 * (k + 8) + y / 8]);
        for (int i = 0; i < 6; i++) {
            vector alpha = _mm256_set1_ps(a[x * n + i * n + k]);//a[x * ny + k * 6 + i]);
            for (int j = 0; j < 2; j++)
                t[i][j] += alpha * b[n / 8 * k + y / 8 + j];//b[(y / 16 * 2 * nx + k * 2 + j)];
                // b[idx_b(k, y + 8 * j, nx) / 8];
        }
    }

    // props to C++17 committee for deprecating "register" keyword
    
    /*
    #define C(i,j) c[x * ny / 8 + i * ny / 8 + y / 8 + j] += t[i][j];
    C(0, 0)
    C(0, 1)
    C(1, 0)
    C(1, 1)
    C(2, 0)
    C(2, 1)
    C(3, 0)
    C(3, 1)
    C(4, 0)
    C(4, 1)
    C(5, 0)
    C(5, 1)
    */

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            c[x * n / 8 + i * n / 8 + y / 8 + j] += t[i][j];
}

const int L1 = (1<<15) / 4; // L1 cache is 32K
const int L2 = (1<<19) / 4; // L2 cache is 512K
const int L3 = (1<<23) / 4; // L3 cache is 8M

void matmul(const float *_a, const float *_b, float *_c, int n) {
    int nx = (n + 5) / 6 * 6;
    int ny = (n + 15) / 16 * 16;

    /*
    float *a = alloc(nx * ny);
    float *b = alloc(nx * ny);
    float *c = alloc(nx * ny);
    */
    
    const int MAXN = 1920 * 1920; // ~15MB each
    alignas(64) static float a[MAXN], b[MAXN], c[MAXN];

    memset(c, 0, 4 * nx * ny);

    //auto a = new(std::align_val_t{64}) float[nx * ny]();
    //auto b = new(std::align_val_t{64}) float[nx * ny]();
    //auto c = new(std::align_val_t{64}) float[nx * ny]();

    //float a[nx * ny] = {0};
    //float b[nx * ny] = {0};
    //float c[nx * ny] = {0};

    for (int i = 0; i < n; i++) {
        memcpy(&a[i * ny], &_a[i * n], 4 * n);
        memcpy(&b[i * ny], &_b[i * n], 4 * n);
    }

    /*
    for (int i = 0; i < n; i += 6)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < std::min(6, n - i); k++)
                a[i * ny + j * 6 + k] = _a[(i+k) * n + j];
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j += 16)
            for (int k = 0; k < std::min(16, n - j); k++)
                b[j * nx + i * 16 + k] = _b[i * n + j + k];
    */

    // c[x:x+6][y:y+16] += a[x:x+6][l:r] * b[l:r][y:y+16]

    // load b[i*L1 : (i+1)*L1][y:y+16] into L1 cache and iterate over a
    // when out of L2 cache to hold a, load new strip of b and continue
    // when out of L3 cache to hold b, switch to new segment of a

    // divide b into segments that fit L3, fix a segment
    // divide a into segments that fit L2, fix a segment
    // divide b into segments that fit L1, fix a segment
    // iterate over a

    /*
    // how many columns of b fit in L3
    const int s3 = std::min(L3 / nx / 16 * 16, ny);
    // how many rows of a fit in L2
    const int s2 = std::min(L2 / ny / 6 * 6, nx);
    // how tall a (k x s3) block in b can be to fit in L1
    const int s1 = std::min(L1 / s3, nx);
    */

   // s3 * nx < L3 (doesn't really matter)
   // s2 * ny < L2
   // s1 * s3 < L1
   // s1 -> max

    //const int s1 = std::min(L1 / 16, nx);
    //const int s2 = L2 / ny / 6 * 6;
    //const int s3 = 16;

    const int u = 96;
    const int s3 = u;
    const int s2 = 2 * u;
    const int s1 = 4 * u;
    
    for (int i3 = 0; i3 < ny; i3 += s3)
        // now we are working with b[:][i3:i3+s3]
        for (int i2 = 0; i2 < nx; i2 += s2)
            // now we are working with a[i2:i2+s2][:]
            for (int i1 = 0; i1 < ny; i1 += s1)
                // now we are working with b[i1:i1+s1][i3:i3+s3]
                // this equates to updating c[i2:i2+s2][i3:i3+s3]
                // with [l:r] = [i1:i1+s1]
                for (int x = i2; x < std::min(i2 + s2, nx); x += 6)
                    for (int y = i3; y < std::min(i3 + s3, ny); y += 16)
                        kernel(a, (vector*) b, (vector*) c, x, y, i1, std::min(i1 + s1, n), ny);

    for (int i = 0; i < n; i++)
        memcpy(&_c[i * n], &c[i * ny], 4 * n);
    
    /*
    delete[] a;
    delete[] b;
    delete[] c;
    */
}
