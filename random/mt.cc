#include "random.hh"
using namespace std;

mt19937 state;

void seed(int s) {
    state = mt19937(s);
}

int rng(int l, int r) {
    return uniform_int_distribution(l, r - 1)(state);
}
