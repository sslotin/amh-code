#include "random.hh"
using namespace std;

typedef float v8si __attribute__ (( vector_size(32) ));

const int state_size = 32;

int state[state_size];
int cnt;

void seed(int s) {
    for (int i = 0; i < state_size; i++)
        state[i] = s + i;
    cnt = 0;
}

int rng(int l, int r) {
    if (cnt == 0) {
        for (int i = 0; i < state_size; i++) {
            state[i] = (state[i] * 1103515245 + 12345) & 0x7fffffff;
            state[i] = state[i] % (r - l) + l;
        }
        cnt = state_size;
    }
    return state[--cnt];// % (r - l) + l;
}
