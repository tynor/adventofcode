#include "aoc.h"

#define S(s) #s, (sizeof(#s)-1)

ssize_t getdigit(struct str s, ssize_t i)
{
    ssize_t rem = s.len - i;
    if (rem >= 5) {
        rem = 5;
    }
    switch (rem) {
    case 5:
        if (!memcmp(s.data+i, S(three))) {
            return 3;
        }
        if (!memcmp(s.data+i, S(seven))) {
            return 7;
        }
        if (!memcmp(s.data+i, S(eight))) {
            return 8;
        }
    case 4:
        if (!memcmp(s.data+i, S(four))) {
            return 4;
        }
        if (!memcmp(s.data+i, S(five))) {
            return 5;
        }
        if (!memcmp(s.data+i, S(nine))) {
            return 9;
        }
    case 3:
        if (!memcmp(s.data+i, S(one))) {
            return 1;
        }
        if (!memcmp(s.data+i, S(two))) {
            return 2;
        }
        if (!memcmp(s.data+i, S(six))) {
            return 6;
        }
    default:
        if ('0' <= s.data[i] && s.data[i] <= '9') {
            return s.data[i] - '0';
        }
    }
    return -1;
}

int main(void)
{
    ssize_t sum = 0;
    struct lines l = {0};
    for (;;) {
        struct str line = readline(&l);
        if (!line.data) {
            break;
        }
        ssize_t first = -1;
        ssize_t last = 0;
        for (ssize_t i = 0; i < line.len; ++i) {
            ssize_t digit = getdigit(line, i);
            if (digit != -1) {
                if (first == -1) {
                    first = digit;
                    last = digit;
                } else {
                    last = digit;
                }
            }
        }
        if (first != -1) {
            ssize_t n = first * 10 + last;
            sum += n;
        }
    }
    printf("%zd\n", sum);
}

#include "aoc.c"
