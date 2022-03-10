int main() {
    volatile bool cond = false;

    for (int t = 0; t < 10; t++) {
        #pragma GCC unroll 3000
        for (int i = 0; i < 3000; i++)
            if (cond) [[likely]]
                break;
    }

    return 0;
}
