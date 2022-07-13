#include "binsearch.hh"

int n;
int *t;

void prepare(int *a, int _n) {
    t = new int[n = _n];
    memcpy(t, a, 4 * n);
}

/*
int lower_bound(int x) {
    int *base = t, len = n;
    while (len > 1) {
        int half = len / 2;
        base = (base[half] < x ? &base[half] : base);
        len -= half;
    }
    return *(base + (*base < x));
}
*/

/*
int lower_bound(int x) {
    int *base = t, len = n;
    while (len > 1) {
        int half = len / 2;
        base += (base[half - (~len & 1)] < x) * half;
        len -= half;
    }
    return *base;
}
*/

int lower_bound(int x) {
    int *base = t, len = n;
    while (len > 1) {
        base += (base[(len - 1) / 2] < x) * (len / 2);
        len = (len + 1) / 2;
    }
    return *base;
}

/*
int lower_bound(int x) {
    int *base = t, len = n;
    while (len > 1) {
        //int half = len / 2;
        //int half1 = (len - 1) / 2;
        //base += (base[half1] < x ? half : 0);
        //len -= half;
        
        int half = (len - 1) >> 1;
        int new_len = (len + 1) >> 1;
        base += (base[half] < x) * (len - new_len); //new_base + new_len = len
        len = new_len;

        //std::cout << len << std::endl;
    }
    return *base;
}
*/
