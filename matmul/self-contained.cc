#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,fma")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <errno.h> // needed for the extern int errno;

#ifndef __cplusplus // Dependencies that are specific to C (as opposed to C++)
#include <stdalign.h> // for aligned memory allocations (needed by SIMD)
#endif

// C version of C++'s std::min().
#define MIN(a, b) ({ \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; \
})

#define KERNEL_ROWS 6
#define KERNEL_COLS 2

// These may be fine-tuned for different target architectures and cache sizes.
#define L3_STEP 64  // How many columns of matrix B to select.
#define L2_STEP 120 // How many rows of matrix A to select.
#define L1_STEP 240 // How many rows of matrix B to select.

// Documented here: https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html
//
// v8sf means that this vector is a vector of 8 units of 4 bytes.  In other
// words, this vector is 32 (8x4) bytes wide and is divided into 8 float-sized
// units that are 4 bytes each.
typedef float v8sf __attribute__ (( vector_size(8 * sizeof (float)) ));


// A 6x16 micro-kernel to benefit from instruction-level SIMD parallelism.
//
// NOTE: While these for-loops are nested, they are also quite short in length.
// For this exact reason, they (and the 2-D SIMD vectors) will automatically be
// unrolled by the compiler (as defined in the #pragma) for greater performance.
//
// This is a shortcut way in C to force an inline "variable" into a register.
static const v8sf _FORCE_INTO_REGISTER = {};
__attribute__ ((optimize("unroll-loops"))) static inline void kernel(
    const float* mat_a, const v8sf* mat_b, v8sf* __restrict__ result,
    const int x, const int y, const int l, const int r, const int n 
) {
    // Zero'ed initially, and then stored in the YMM (on x86) registers.
    v8sf simd_registers[KERNEL_ROWS][KERNEL_COLS] = {0};
    
    // NOTE: Use signed only; other types (e.g., uint_fast32_t, unsigned int, or
    // uint32_t) prevent the Compiler from making the (rightful) assumption that
    // overflows are undefined, especially in the first for-loop.
    // Additionally, using int_fastN_t will leave it to the Compiler to  use the
    // fastest available register.  This is usually a long int on x86 (so there
    // won't be any saving on space) but this may still benefit smaller systems
    // such as ARM where the cache space is scarce and the fastest available
    // register may actually be smaller than a long int or even an int.
    for (int_fast32_t k = l; k < r; ++k)
        for (int_fast8_t  i = 0; i < KERNEL_ROWS; ++i)
            for (int_fast8_t j = 0; j < KERNEL_COLS; ++j) {
                // This forces mat_a[x + i][k] into a register, and 
                // multiplies mat_b[k][y:y+16] by it, then updates
                // simd_registers[i][0] and simd_registers[i][1].
                // 
                // Gets converted to an FMA (Fused Multiply-Add) by GCC.
                simd_registers[i][j] += (_FORCE_INTO_REGISTER + mat_a[(x + i)*n + k]) * ((v8sf*)mat_b)[(k*n + y)/8 + j];
            }

    // Writes the sub-matrix results back from the vector registers.
    for (int_fast8_t i = 0; i < KERNEL_ROWS; ++i)
        for (int_fast8_t j = 0; j < KERNEL_COLS; ++j)
            ((v8sf*)result)[(((x + i)*n + y)/8) + j] += simd_registers[i][j];
}

// Helper function to allocate and zero a region of memory (similar to calloc())
// but in an aligned manner for 32-bit floats. (Also error-checked)
float* float_aligned_calloc(const unsigned int size) {
    alignas(64) float* ptr = (float*) aligned_alloc(64, size * sizeof (*ptr));
    // aligned_alloc() returns NULL on failure and/or 0-sized allocations.
    if (ptr == NULL && size != 0) { 
        // (If any) prints the error code with its message to stderr.
        if (errno != 0) {
            fprintf(stderr, \
                "Error no. `%d` in aligned memory allocation was "
                "encountered: \"%s\"\n ", errno, strerror(errno) \
            );
        }
        // Memory allocation was not successful, so we return NULL to indicate
        // that an error had occured.
        return NULL;
    }
    
    memset(ptr, 0, size * sizeof (*ptr));
    // The requested memory was successfully allocated and will be returned.
    return ptr;
}

// Multiplies two square matrices of the same width and height.
// Takes two pointers to said matrices, as well as their "dimension" (i.e.,
// width or height)
float* matmul(const float *mat_a, const float *mat_b, int n) {
    // To simplify the implementation, we pad the height and width,
    // so that they are divisible by 6 and 16, respectively.
    int nx = ((n + 5)/6) * 6;
    int ny = ((n + 15)/16) * 16;
        
    // SIMD operartions need to be able to access their memory regions with
    // aligned read/write operations, otherwise the program could crash.
    float *aligned_mat_a = float_aligned_calloc(nx * ny);
    float *aligned_mat_b = float_aligned_calloc(nx * ny);
    float *aligned_result = float_aligned_calloc(nx * ny);
    
    // Copies the non-aligned input arrays into the new, aligned ones.
    for (int i = 0; i < n; ++i) {
        memcpy(&aligned_mat_a[i * ny], &mat_a[i * n], sizeof (*aligned_mat_a) * n);
        memcpy(&aligned_mat_b[i * ny], &mat_b[i * n], sizeof (*aligned_mat_b) * n);
    }

    // The outermost loops comprise the Macro Kernel.
    for (int i3 = 0; i3 < ny; i3 += L3_STEP)
        // We are now working with mat_b[:][i3:i3+L3_STEP].
        for (int i2 = 0; i2 < nx; i2 += L2_STEP)
        // We are now working with mat_aa[i2:i2+L2_STEP][:].
            for (int i1 = 0; i1 < ny; i1 += L1_STEP)
                // We are now working with b[i1:i1+s1][i3:i3+s3].
                // This equates to updating c[i2:i2+s2][i3:i3+s3]
                // with [l:r] = [i1:i1+s1]
                for (int x = i2; x < MIN(i2 + L2_STEP, n); x += 6)
                    for (int y = i3; y < MIN(i3 + L3_STEP, n); y += 16) {
                        kernel(
                            aligned_mat_a, (v8sf*)aligned_mat_b, (v8sf*)aligned_result,
                            x, y, i1, MIN(i1 + L1_STEP, n), ny
                        );
                    }

    float* result = (float*) malloc(n*n * sizeof (*result));
    for (int i = 0; i < n; ++i)
        memcpy(&result[i * n], &aligned_result[i * ny], n * sizeof (*result));
    
    // Frees the previous temporary arrays as we are done working with them.
    free(aligned_mat_a);
    free(aligned_mat_b);
    free(aligned_result);

    return (float*) result;
}

// A very slow but correct method for double-checking the results from matmul()
// and benchmarks' comparison.
float* naive(const float* mat_a, const float* mat_b, const int n) {
    float *result = (float*) calloc(n * n, sizeof (result));
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                result[i*n + j] += mat_a[i*n + k] * mat_b[k*n + j];
                
    return result;
}

static int idx[5];
float* timeit(
    float* (*f)(const float*, const float*, const int),
    const float* mat_a, const float* mat_b, const int n
) {
    float *result_matrix = f(mat_a, mat_b, n);
    // Selects 5 randomly pre-determined elements of the matrices to be shown.
    for (int i = 0; i < 5; ++i)
        printf("%.4f ", result_matrix[idx[i] % (n * n)]);
    printf("\n");

    clock_t start = clock();
    int cnt = 0;
    while (clock() - start < CLOCKS_PER_SEC)
        f(mat_a, mat_b, n), cnt++;
    printf("Took %.6f x %d\n", (float)(clock() - start) / CLOCKS_PER_SEC / cnt, cnt);
    
    return result_matrix;
}

// Test Driver
int main() {
    const int n = 1920; // Matrix of size n x n (n needs to be a natural number.)
    
    // These are just test samples.  They should be allocated on the heap if
    // their sizes are large, otherwise an stack overflow will happen.
    float *mat_a = (float*) malloc(sizeof (*mat_a) * n*n);
    float *mat_b = (float*) malloc(sizeof (*mat_b) * n*n);
    
    for (int i = 0; i < n*n; ++i) {
        mat_a[i] = (float)rand() / RAND_MAX;
        mat_b[i] = (float)rand() / RAND_MAX;
    }
    
    // idx is a static global variable that timit() will use to randomly print
    // 5 elements of its callee's result.
    for (int i = 0; i < 5; ++i)
        idx[i] = rand();

    printf("Running matmul() on a matrix of size %dx%d...\n", n, n);
    float* matmul_res = timeit(matmul, mat_a, mat_b, n);
    
    printf("\n");
    
    printf("Running naive() on a matrix of size %dx%d....\n", n, n);
    float* naive_res = timeit(naive, mat_a, mat_b, n);
    
    printf("\n\n");
    
    printf("Calculations finished, comparing all the results now...\n");
    // Compares each and every element of the results of both naive() and
    // matmul() and reports if a mismatch was found (due to the use of -Ofast,
    // an error/difference of up to 0.0010f is tolerated.)
    for (int i = 0; i < n*n; ++i) {
        if (abs(matmul_res[i] - naive_res[i]) > 0.0010f) {
            printf(
                "Mismatching element found: %.4f and %.4f at index %d with a delta of %f.\n", 
                matmul_res[i], naive_res[i], i, matmul_res[i] - naive_res[i]
            );
            return EXIT_FAILURE;
        }
    }
    printf("Both results were fully matching.\n");

    free(matmul_res);
    free(naive_res);

    return EXIT_SUCCESS;
}
