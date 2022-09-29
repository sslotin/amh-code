#include "read.hh"

static const int buf_size = (1<<12);
static unsigned char buf[buf_size];
static int buf_len = 0, buf_pos = 0;

inline bool isEof() {
    if (buf_pos == buf_len) {
        buf_pos = 0;
        buf_len = fread(buf, 1, buf_size, stdin);
        if (buf_pos == buf_len)
            return true;
    }
    return false;
}

inline int getChar() { 
    return isEof() ? -1 : buf[buf_pos++];
}

inline int readChar() {
    int c = getChar();
    while (c != -1 && c <= 32)
        c = getChar();
    return c;
}


inline int readUInt() {
    int c = readChar(), x = 0;
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return x;
}

int read(int n) {
    int s = 0;

    for (int i = 0; i < n; i++) {
        int x = readUInt();
        s ^= x;
    }

    return s;
}
