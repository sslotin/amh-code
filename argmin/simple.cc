#include "argmin.hh"

int argmin() {
    int k = 0;
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (a[i] < a[k])
            k = i, cnt++;
    return k;
}
