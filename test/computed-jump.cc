int main() {
    volatile int x = 0;

    for (int i = 0; i < 1000000; i++) {
        switch (x) {
            case 0:
                x = 1;
                break;
            case 1:
                x = 2;
                break;
            case 2:
                x = 3;
                break;
            case 3:
                x = 0;
                break;
        }
    }

    return 0;
}
