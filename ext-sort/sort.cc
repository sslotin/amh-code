#include <bits/stdc++.h>

const int B = (1<<20) / 4; // 1 MB blocks of integers
const int M = (1<<28) / 4; // available memory

const char* finput = "large.bin";
const char* foutput = "output.bin";

int main() {
    FILE *input = fopen(finput, "rb");

    std::vector<FILE*> parts;

    while (true) {
        static int part[M]; // better delete it right after
        int n = fread(part, 4, M, input);

        if (n == 0)
            break;
        
        std::sort(part, part + n);
        
        char fpart[sizeof "part-999.bin"];
        sprintf(fpart, "part-%03d.bin", parts.size());

        printf("Writing %d elements into %s...\n", n, fpart);

        FILE *file = fopen(fpart, "wb");
        fwrite(part, 4, n, file);
        fclose(file);
        
        file = fopen(fpart, "rb");
        parts.push_back(file);
    }

    fclose(input);

    std::priority_queue< std::pair<int, int> > q;

    const int nparts = parts.size();
    auto buffers = new int[nparts][B];
    int outbuffer[B];
    std::vector<int> l(nparts), r(nparts);

    for (int part = 0; part < nparts; part++) {
        r[part] = fread(buffers[part], 4, B, parts[part]);
        q.push({buffers[part][0], part});
        l[part] = 1;
    }

    FILE *output = fopen(foutput, "w");
    
    int buffered = 0;

    while (!q.empty()) {
        auto [key, part] = q.top();
        q.pop();

        outbuffer[buffered++] = key;
        if (buffered == B) {
            fwrite(outbuffer, 4, B, output);
            buffered = 0;
        }

        if (l[part] == r[part]) {
            r[part] = fread(buffers[part], 4, B, parts[part]);
            l[part] = 0;
        }

        if (l[part] < r[part]) {
            q.push({buffers[part][l[part]], part});
            l[part]++;
        }
    }

    fwrite(outbuffer, 4, buffered, output);

    delete[] buffers;
    for (FILE *file : parts)
        fclose(file);
    
    fclose(output);

    return 0;
}