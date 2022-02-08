#include "argmin.hh"
#include <x86intrin.h>

const int B = 8;
typedef int vec __attribute__ (( vector_size(4 * B) ));

int argmin(int *a, int n) {
    vec *v = (vec*) a;

    vec min = INT_MAX + vec{};
    vec idx, cur = {0, 1, 2, 3, 4, 5, 6, 7};

    // blend    01
    // vpcmpgtd 013
    // vpminsd  013
    // add      013

    // 15 GFLOPS @ 4.1G = 3.6 values / cycle?
    
    /*
        vpcmpgtd  ymm2, ymm0, YMMWORD PTR [rax]
        vpblendvb ymm3, ymm4, ymm1, ymm2
        vpblendvb ymm0, ymm0, YMMWORD PTR [rax-32], ymm2
        vpaddd    ymm1, ymm1, ymm5
        vmovdqa   ymm4, ymm3
    */
    
    // 8 * (3 / 5) = 4.8

    for (int i = 0; i < n / B; i++) {
        vec mask = (v[i] < min);
        idx = (mask ? cur : idx);
        min = (mask ? v[i] : min);
        cur += B;
    }
    
    int k = 0, m = min[0];

    for (int i = 1; i < B; i++)
        if (min[i] < m)
            m = min[k = i];

    return idx[k];
}
