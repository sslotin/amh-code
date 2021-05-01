#include "random.hh"
using namespace std;

void seed(int s) {
    srand(s);
}

int rng(int l, int r) {
    return rand() % (r - l) + l;
}
