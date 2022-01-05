#include <bits/stdc++.h>
using namespace std;

/*
template<int N, int... block_sizes>
struct Test {
    static constexpr int B[sizeof...(block_sizes)] = {block_sizes...};
    void print() {
        for (int x : B)
            cout << x << endl;
    }
};
*/

// this requires C++20
struct Layer {
    /*
    int size;      // size of blocks on cache layer
    bool permuted; // whether the blocks there are permuted (3-4 cycles faster)
    bool prefetch; // prefetch the first possible child of the next layer
                   //  (not necessarily belonging to the cache line we are going to read)
    */
    //constexpr Layer(int size, bool permuted, bool prefetch)
    //              : size(size), permuted(permuted), prefetch(prefetch) {}
    constexpr Layer() {}
    virtual void permute() const = 0;
    //virtual void rank();
};

struct LayerA : public Layer {
    //constexpr Layer(int size, bool permuted, bool prefetch)
    //              : size(size), permuted(permuted), prefetch(prefetch) {}
    void permute() const final { cout << "A" << endl; };
};

struct LayerB : public Layer {
    //constexpr Layer(int size, bool permuted, bool prefetch)
    //              : size(size), permuted(permuted), prefetch(prefetch) {}
    void permute() const final { cout << "B" << endl; };
};

typedef Layer* pLayer;

//template<int N, int... block_sizes>
struct Test {
    static constexpr LayerA l1();
    static constexpr LayerB l2();

    static constexpr pLayer L[2] = {&l1, &l2};
    //constexpr Test() {
    //}
    /*
    constexpr Test() {
        L[0] = LayerA();
        L[1] = LayerB();
        L[2] = LayerA();
    }*/
    //static constexpr int B[sizeof...(block_sizes)] = {block_sizes...};
    void print() const {
        for (int i = 0; i < 3; i++)
            L[i]->permute();
    }
};

int main() {
    Test t;
    t.print();

    return 0;
}
