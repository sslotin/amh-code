#include "gcd.hh"

int gcd(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;

    int az = __builtin_ctz(a);
    int bz = __builtin_ctz(b);
    int shift = std::min(az, bz);
    b >>= bz;
    
    while (a != 0) {
        a >>= az;
        int diff = b - a;
        az = __builtin_ctz(diff);
        b = std::min(a, b);
        a = std::abs(diff);
    }
    
    return b << shift;
}

/*
int gcd(int u, int v) {
  int shift, uz, vz;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  uz = __builtin_ctz(u);
  vz = __builtin_ctz(v);
  shift = uz > vz ? vz : uz;
  u >>= uz;
  do {
    v >>= vz;
    int diff = v;
    diff -= u;
    vz = __builtin_ctz(diff);
    if (diff == 0)
      break;
    if (v < u)
      u = v;
    v = abs(diff);

  } while (1);
  return u << shift;
}
*/
