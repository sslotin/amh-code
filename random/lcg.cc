#include "random.hh"
using namespace std;

int state;

void seed(int s) {
    state = s;
}

int rng(int l, int r) {
    state = (1ll * state * 1103515245 + 12345) & 0x7fffffff;
    return state % (r - l) + l;
}

/*
    int32_t val = ((state[0] * 1103515245U) + 12345U) & 0x7fffffff;
    state[0] = val;
    *result = val;
*/

/*
int rng(int l, int r) {
    state = state * 214013L + 2531011L;
    ((
        (holdrand = holdrand * 214013L + 2531011L) >> 16)
        & 0x7fff);
    return state % (r - l) + l;
}
*/
