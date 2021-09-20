#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

char a[32];

int main() {
    for (int i = 0; i < 32; i++)
        a[i] = i;
    
    auto reg = _mm256_loadu_si256((__m256i*) a);
    auto x = _mm256_set1_epi8(0);

    cout << __builtin_ctz(_mm256_movemask_epi8(_mm256_cmpeq_epi8(reg, x))) << endl;

    return 0;
}
