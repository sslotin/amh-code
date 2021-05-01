#include <iostream>
using namespace std;

int main() {
	const char* s[] = {"sse", "sse2", "avx", "avx2", "avx512f"};
	for (const char* x : s)
		printf("%s: %s\n", x, __builtin_cpu_supports(x) ? "yes" : "no");

    cout << __builtin_cpu_supports("sse") << endl;
    cout << __builtin_cpu_supports("sse2") << endl;
    cout << __builtin_cpu_supports("avx") << endl;
    cout << __builtin_cpu_supports("avx2") << endl;
    cout << __builtin_cpu_supports("avx512f") << endl;

    return 0;
}
