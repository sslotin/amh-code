#include "count.hh"

int count(int x) {
    int cnt = 0;
    for (int i = 0; i < N; i++)
        cnt += (a[i] == x);
    return cnt;
}
