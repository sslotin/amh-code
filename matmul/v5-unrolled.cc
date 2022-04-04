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

void kernel(float *a, vector *b, vector *c, int x, int y, int l, int r, int n) {
    vector t00, t01,
           t10, t11,
           t20, t21,
           t30, t31,
           t40, t41,
           t50, t51;

    t00 = c[((x + 0) * n + y) / 8 + 0];
    t01 = c[((x + 0) * n + y) / 8 + 1];

    t10 = c[((x + 1) * n + y) / 8 + 0];
    t11 = c[((x + 1) * n + y) / 8 + 1];

    t20 = c[((x + 2) * n + y) / 8 + 0];
    t21 = c[((x + 2) * n + y) / 8 + 1];

    t30 = c[((x + 3) * n + y) / 8 + 0];
    t31 = c[((x + 3) * n + y) / 8 + 1];

    t40 = c[((x + 4) * n + y) / 8 + 0];
    t41 = c[((x + 4) * n + y) / 8 + 1];

    t50 = c[((x + 5) * n + y) / 8 + 0];
    t51 = c[((x + 5) * n + y) / 8 + 1];

    for (int k = l; k < r; k++) {
        vector a0 = vector{} + a[(x + 0) * n + k];
        t00 += a0 * b[(k * n + y) / 8];
        t01 += a0 * b[(k * n + y) / 8 + 1];

        vector a1 = vector{} + a[(x + 1) * n + k];
        t10 += a1 * b[(k * n + y) / 8];
        t11 += a1 * b[(k * n + y) / 8 + 1];

        vector a2 = vector{} + a[(x + 2) * n + k];
        t20 += a2 * b[(k * n + y) / 8];
        t21 += a2 * b[(k * n + y) / 8 + 1];

        vector a3 = vector{} + a[(x + 3) * n + k];
        t30 += a3 * b[(k * n + y) / 8];
        t31 += a3 * b[(k * n + y) / 8 + 1];

        vector a4 = vector{} + a[(x + 4) * n + k];
        t40 += a4 * b[(k * n + y) / 8];
        t41 += a4 * b[(k * n + y) / 8 + 1];

        vector a5 = vector{} + a[(x + 5) * n + k];
        t50 += a5 * b[(k * n + y) / 8];
        t51 += a5 * b[(k * n + y) / 8 + 1];
    }

    c[((x + 0) * n + y) / 8 + 0] = t00;
    c[((x + 0) * n + y) / 8 + 1] = t01;

    c[((x + 1) * n + y) / 8 + 0] = t10;
    c[((x + 1) * n + y) / 8 + 1] = t11;

    c[((x + 2) * n + y) / 8 + 0] = t20;
    c[((x + 2) * n + y) / 8 + 1] = t21;

    c[((x + 3) * n + y) / 8 + 0] = t30;
    c[((x + 3) * n + y) / 8 + 1] = t31;

    c[((x + 4) * n + y) / 8 + 0] = t40;
    c[((x + 4) * n + y) / 8 + 1] = t41;

    c[((x + 5) * n + y) / 8 + 0] = t50;
    c[((x + 5) * n + y) / 8 + 1] = t51;
}

/*
void kernel(float * __restrict__ a, vector * __restrict__ b, vector * __restrict__ c,
            int x, int y, int l, int r, int n) {
    for (int k = l; k < r; k++) {
        for (int i = 0; i < 6; i++) {
            vector alpha = vector{} + a[(x + i) * n + k];
            for (int j = 0; j < 2; j++)
                c[((x + i) * n + y) / 8 + j] += alpha * b[(k * n + y) / 8 + j];
        }
    }
}
*/

/*
template<typename T>
vector load(T* t) {
    return _mm256_load_ps( (float*) t );
}

void kernel(float *a, vector *b, vector *c, int x, int y, int l, int r, int n) {
    vector t00, t01, t10, t11, t20, t21, t30, t31, t40, t41, t50, t51;

    t00 = load(&c[((x + 0) * n + y) / 8 + 0]);
    t01 = load(&c[((x + 0) * n + y) / 8 + 1]);
    t10 = load(&c[((x + 1) * n + y) / 8 + 0]);
    t11 = load(&c[((x + 1) * n + y) / 8 + 1]);
    t20 = load(&c[((x + 2) * n + y) / 8 + 0]);
    t21 = load(&c[((x + 2) * n + y) / 8 + 1]);
    t30 = load(&c[((x + 3) * n + y) / 8 + 0]);
    t31 = load(&c[((x + 3) * n + y) / 8 + 1]);
    t40 = load(&c[((x + 4) * n + y) / 8 + 0]);
    t41 = load(&c[((x + 4) * n + y) / 8 + 1]);
    t50 = load(&c[((x + 5) * n + y) / 8 + 0]);
    t51 = load(&c[((x + 5) * n + y) / 8 + 1]);

    for (int k = l; k < r; k++) {
        vector b0 = load(&b[(k * n + y) / 8 + 0]);
        vector b1 = load(&b[(k * n + y) / 8 + 1]);

        vector a0 = _mm256_set1_ps(a[(x + 0) * n + k]);
        t00 += a0 * b0;
        t01 += a0 * b1;
        
        vector a1 = _mm256_set1_ps(a[(x + 1) * n + k]);
        t10 += a1 * b0;
        t11 += a1 * b1;

        vector a2 = _mm256_set1_ps(a[(x + 2) * n + k]);
        t20 += a2 * b0;
        t21 += a2 * b1;

        vector a3 = _mm256_set1_ps(a[(x + 3) * n + k]);
        t30 += a3 * b0;
        t31 += a3 * b1;

        vector a4 = _mm256_set1_ps(a[(x + 4) * n + k]);
        t40 += a4 * b0;
        t41 += a4 * b1;

        vector a5 = _mm256_set1_ps(a[(x + 5) * n + k]);
        t50 += a5 * b0;
        t51 += a5 * b1;
    }

    c[((x + 0) * n + y) / 8 + 0] = t00;
    c[((x + 0) * n + y) / 8 + 1] = t01;
    c[((x + 1) * n + y) / 8 + 0] = t10;
    c[((x + 1) * n + y) / 8 + 1] = t11;
    c[((x + 2) * n + y) / 8 + 0] = t20;
    c[((x + 2) * n + y) / 8 + 1] = t21;
    c[((x + 3) * n + y) / 8 + 0] = t30;
    c[((x + 3) * n + y) / 8 + 1] = t31;
    c[((x + 4) * n + y) / 8 + 0] = t40;
    c[((x + 4) * n + y) / 8 + 1] = t41;
    c[((x + 5) * n + y) / 8 + 0] = t50;
    c[((x + 5) * n + y) / 8 + 1] = t51;
}
*/

/*
const int L1 = (1<<15) / 4; // L1 cache is 32K
const int L2 = (1<<19) / 4; // L2 cache is 512K
const int L3 = (1<<23) / 4; // L3 cache is 8M
*/

void matmul(const float *_a, const float *_b, float *_c, int n) {
    int nx = (n + 5) / 6 * 6;
    int ny = (n + 15) / 16 * 16;
    
    const int MAXN = 1920 * 1920; // ~15MB each
    alignas(64) static float a[MAXN], b[MAXN], c[MAXN];

    /*for (int i = 0; i < n; i++) {
        memcpy(&a[i * ny], &_a[i * n], 4 * n);
        memcpy(&b[i * ny], &_b[i * n], 4 * n);
    }*/

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

    /*
    const int s3 = 64;
    const int s2 = 120;
    const int s1 = 240;
    */

    const int u = 96;
    const int s3 = u;
    const int s2 = 2 * u;
    const int s1 = 4 * u;

    //const int t = L1/s3;

    // 1 252 4032
    //std::cerr << s1 << " " << s2 << " " << s3 << std::endl;

    for (int i3 = 0; i3 < ny; i3 += s3)
        // now we are working with b[:][i3:i3+s3]
        for (int i2 = 0; i2 < nx; i2 += s2)
            // now we are working with a[i2:i2+s2][:]
            for (int i1 = 0; i1 < ny; i1 += s1)
                // now we are working with b[i1:i1+s1][i3:i3+s3]
                // this equates to updating c[i2:i2+s2][i3:i3+s3]
                // with [l:r] = [i1:i1+s1]
                for (int x = i2; x < i2 + s2; x += 6)
                    for (int y = i3; y < i3 + s3; y += 16)
                        kernel(a, (vector*) b, (vector*) c, x, y, i1, i1 + s1, ny);

    //for (int i = 0; i < n; i++)
    //    memcpy(&_c[i * n], &c[i * ny], 4 * n);
}
