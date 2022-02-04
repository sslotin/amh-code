#include "find.hh"

int find(int x) {
    for (int i = 0; i < N; i++)
        if (a[i] == x)
            return i;
    return -1;
}
