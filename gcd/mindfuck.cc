#include "gcd.hh"

int gcd(int a, int b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}