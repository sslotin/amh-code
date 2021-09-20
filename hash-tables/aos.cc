#include "hashmap.hh"

const float alpha = 0.25;

int filled = 0, capacity = 1024;

struct element { int x, y; };
element *t;

void setup() {
    t = new element[capacity];
    memset(t, -1, 8 * capacity);
}

void rehash() {
    cerr << "rehash " << capacity << endl;

    element *_t = t;

    filled = 0;
    capacity *= 2;
    
    t = new element[capacity];

    memset(t, -1, 8 * capacity);

    for (int i = 0; i < capacity / 2; i++)
        if (_t[i].x != -1)
            add(_t[i].x, _t[i].y);

    delete _t;
}

int pos(int k) { return k & (capacity - 1); }

void add(int x, int y) {
    if (filled + 1 > alpha * capacity)
        rehash();

    int k = x & (capacity - 1);
    
    while (t[k].x != -1 && t[k].x != x)
        k = (k + 1) & (capacity - 1);
    
    t[k] = {x, y};
    filled++;
}

int get(int x) {
    for (int k = pos(x); t[k].x != -1; k = pos(k + 1))
        if (t[k].x == x)
            return t[k].y;
    return -1;
}
