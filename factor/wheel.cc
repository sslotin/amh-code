#include "factor.hh"

/*
u64 find_factor(u64 n) {
    if (n % 2 == 0)
        return 2;
    for (u64 d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return d;
    return 1;
}*/

/*
u64 find_factor(u64 n) {
    for (u64 d : {2, 3, 5})
        if (n % d == 0)
            return d;
    //u64 increments[] = {4, 2, 4, 2, 4, 6, 2, 6};
    u64 increments[] =   {0, 4, 6, 10, 12, 16, 22, 24};
    u64 sum = 30;
    for (u64 d = 7; d * d <= n; d += sum) {
        for (u64 k = 0; k < 8; k++) {
            u64 x = d + increments[k];
            if (n % x == 0)
                return x;
        }
    }
    return 1;
}*/

/*
u64 find_factor(u64 n) {
    for (u64 d : {2, 3, 5})
        if (n % d == 0)
            return d;
    u64 increments[] = {4, 2, 4, 2, 4, 6, 2, 6};
    for (u64 d = 7, k = 0; d * d <= n; d += increments[k++ % 8])
        if (n % d == 0)
            return d;
    return 1;
}*/

/*
const int primes[] = {2, 3, 5, 7, 11, 13};

struct Wheel {
    int skiplist[];
    vector<int> skiplist; // {1, 17, ...}
    int product = 1;

    constexpr Wheel() : skiplist{} {
        for (int p : primes)
            product *= p;
        for (int i = 1; i < product; i++) {
            bool skip = false;
            for (int p : primes)
                if (i % p == 0)
                    skip = true;
            if (!skip)
                skiplist.push_back(i);
        }
    }
};

constexpr Wheel wheel{};

u64 find_factor(u64 n) {
    for (int p : primes)
        if (n % p == 0)
            return p;
    for (u64 base = 0; base * base < n; base += wheel.product)
        for (u64 offset : wheel.skiplist)
            if (n % (base + offset) == 0)
                if (base + offset > 1)
                    return base + offset;
    return 1;
}
*/
