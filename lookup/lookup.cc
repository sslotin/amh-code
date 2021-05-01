#include <bits/stdc++.h>
using namespace std;

using ResultT = int;
constexpr ResultT f(int i)
{
    return i * 2;
}

constexpr auto LUT = []
{
    constexpr auto LUT_Size = 1024;
    std::array<ResultT, LUT_Size> arr = {};

    for (int i = 0; i < LUT_Size; ++i)
    {
        arr[i] = f(i);
    }

    return arr;
}();

static_assert(LUT[100] == 200);

int main() {

}
