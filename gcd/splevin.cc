#include "gcd.hh"

int gcd(int u, int v) {
  int shift;
  //assert(v <= INT_MAX);
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    unsigned m;
    v >>= __builtin_ctz(v);
    v -= u;
    m = (int)v >> 31;
    u += v & m; /* u + (v - u) = v */
    v = (v + m) ^ m;
  } while (v != 0);
  return u << shift;
}