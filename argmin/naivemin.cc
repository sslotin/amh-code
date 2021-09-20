#include "argmin.hh"

int argmin() {
    int m = INT_MAX;

    for (int i = 0; i < n; i++)
        if (a[i] < m)
            m = a[i];

    return m;
}
