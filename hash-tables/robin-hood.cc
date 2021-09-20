#include "hashmap.hh"

const float alpha = 0.5;

int filled = 0, capacity = 1024;
int *a, *b;

void setup() {
    a = new int[capacity];
    b = new int[capacity];
    memset(a, -1, 4 * capacity);
}

void rehash() {
    cerr << "rehash " << capacity << endl;

    int *_a = a, *_b = b;

    filled = 0;
    capacity *= 2;
    
    a = new int[capacity];
    b = new int[capacity];

    memset(a, -1, 4 * capacity);

    for (int i = 0; i < capacity / 2; i++)
        if (_a[i] != -1)
            add(_a[i], _b[i]);

    delete _a;
    delete _b;
}

int pos(int k) { return k & (capacity - 1); }
int dist(int x, int k) {
    return pos(x) < k ? k - pos(x) : k + capacity - pos(x);
}

void add(int x, int y) {
    if (filled + 1 > alpha * capacity)
        rehash();

    int k = pos(x);

    while (a[k] != -1 && a[k] != x) {
        if (dist(a[k], k) < dist(x, k)) {
            swap(a[k], x);
            swap(b[k], y);
        }
        k = pos(k + 1);
    }
    
    a[k] = x, b[k] = y;
    filled++;
}

int get(int x) {
    for (int k = pos(x); a[k] != -1; k = pos(k + 1))
        if (a[k] == x)
            return b[k];
    return -1;
}
