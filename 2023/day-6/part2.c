#include "aoc.c"

ssize_t parse_spread_int(struct str s)
{
    ssize_t n = 0;
    ssize_t i = 0;
    while (i != s.len && s.data[i] != '\n') {
        while (i != s.len && !isdigit(s.data[i])) {
            ++i;
        }
        for (; i != s.len && isdigit(s.data[i]); ++i) {
            n *= 10;
            n += s.data[i] - '0';
        }
    }
    return n;
}

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1UL<<32));
    struct lines l = {0};
    ssize_t ms = parse_spread_int(readline(&l));
    ssize_t record = parse_spread_int(readline(&l));
    ssize_t min_ms = -1;
    for (ssize_t i = 1; i < ms; ++i) {
        ssize_t dist = (ms - i) * i;
        if (dist > record) {
            min_ms = i;
            break;
        }
    }
    ssize_t max_ms = -1;
    for (ssize_t i = ms - 1; i; --i) {
        ssize_t dist = (ms - i) * i;
        if (dist > record) {
            max_ms = i;
            break;
        }
    }
    printf("%zd\n", max_ms - min_ms + 1);
}
