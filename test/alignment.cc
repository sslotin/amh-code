#include <iostream>
using namespace std;

// 8, 16
struct NodeA {
    NodeA *left, *right;
};

// 16, 20, 24
struct NodeB {
    int sum;
    NodeB *left, *right;
};

struct NodeB1 {
    int sum;
    char padding[4];
    NodeB *left, *right;
};

struct NodeB2 {
    NodeB *left, *right;
    int sum;
    char padding[4];
};

struct NodeC {
    int sum;
    bool reversed;
    NodeB *left, *right;
};

struct NodeD {
    //int a : 24;
    //bool b;
    NodeB *left, *right;
    long long sum;
    bool reversed;
    int t;
    int key, priority;
};

struct Node48 {
    int key;
    NodeB *left, *right;
    int priority;
    long long sum;
    bool reversed;
};


struct NodeE {
    int sum;
    NodeB *left, *right;
    bool reversed;
};

struct alignas(64) NodeF {
    int* i1;
    bool b1;
    int* i2;
    bool b2;
    int* i3;
    bool b3;
    int* i4;
    bool b4;
    int* i5;
    bool b5;
};


struct NodeG {
    int* i1;
    int* i2;
    int* i3;
    int* i4;
    int* i5;
    bool b1;
    bool b2;
    bool b3;
    bool b4;
    bool b5;
};


int main() {

    //cout << sizeof(NodeA) << endl;
    //cout << sizeof(NodeB) << endl;
    //cout << sizeof(NodeC) << endl;
    //cout << sizeof(NodeD) << endl;
    //cout << alignof(NodeD) << endl;
    //cout << sizeof(NodeE) << endl;
    cout << sizeof(NodeF) << endl;
    cout << alignof(NodeF) << endl;
    cout << sizeof(NodeG) << endl;
    cout << alignof(NodeG) << endl;

    return 0;
}
