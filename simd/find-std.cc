#include "find.hh"

int find(int x) {
    return std::find(a, a + N, x) - a;
}
