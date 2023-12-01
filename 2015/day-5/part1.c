#include "aoc.h"

bool nice_str(struct str s);

int main(void)
{
    struct lines l = {0};
    ssize_t nnice = 0;
    for (;;) {
        struct str line = readline(&l);
        if (!line.data) {
            break;
        }
        nnice += nice_str(line);
    }
    printf("%zd\n", nnice);
}

bool vowel(uint8_t c);

bool banned_combo(uint8_t a, uint8_t b);

bool nice_str(struct str s)
{
    ssize_t nvowels = 0;
    ssize_t hasdouble = 0;
    for (ssize_t i = 0; i < s.len; ++i) {
        nvowels += vowel(s.data[i]);
        if (i < s.len-1) {
            if (banned_combo(s.data[i], s.data[i+1])) {
                return false;
            }
            hasdouble += s.data[i] == s.data[i+1];
        }
    }
    return nvowels >= 3 && hasdouble;
}

bool vowel(uint8_t c)
{
    switch (c) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
    default:
        return false;
    }
}

bool banned_combo(uint8_t a, uint8_t b)
{
    return
        (a == 'a' && b == 'b') ||
        (a == 'c' && b == 'd') ||
        (a == 'p' && b == 'q') ||
        (a == 'x' && b == 'y');
}

#include "aoc.c"
