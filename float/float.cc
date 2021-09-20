#include <bits/stdc++.h>
#include <format>

struct money {
    uint v; // 1/10000th of a dollar
};

std::string to_string(money) {
	return std::format("${0}.{1:04d}", v / 10000, v % 10000);
}

int main() {

    money m = {12345};

    std::cout << to_string(m) << std::endl;

    return 0;
}
