
#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/mman.h>

const int N = 1024;

const char* finput = "input.bin";
const char* foutput = "output.bin";

int main() {
    int fd = open(finput, O_RDWR);

    int* data = (int*) mmap(0, 4 * N, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (int i = 0; i < 10; i++)
        printf("%d\n", data[i]);
    
    std::sort(data, data + N);

    return 0;
}
