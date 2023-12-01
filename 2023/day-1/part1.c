#include "aoc.h"

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
            if ('0' <= line.data[i] && line.data[i] <= '9') {
                ssize_t digit = line.data[i] - '0';
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
