#include "gcd.hh"
using namespace std;

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

/*
int gcd(int u, int v) {
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
    shift = __builtin_ctz(u | v);
    u >>= __builtin_ctz(u);
    do {
        v >>= __builtin_ctz(v);
        if (u > v) {
            unsigned int t = v;
            v = u;
            u = t;
        }  
        v = v - u;
    } while (v != 0);
    return u << shift;
}*/