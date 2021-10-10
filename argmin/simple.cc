#include "argmin.hh"

int argmin() {
    int k = 0;
    for (int i = 0; i < n; i++)
        if (a[i] < a[k])
            k = i;
    return k;
}
