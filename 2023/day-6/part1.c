#include "aoc.c"

struct ints
{
    ssize_t *data;
    ssize_t len;
    ssize_t cap;
};

void parse_nums(struct str line, struct ints *ns, struct arena *a)
{
    while (line.len && !str_eq(line, S_LIT("\n"))) {
        ssize_t n = parse_int(line, &line);
        *push(ns, a) = n;
    }
}

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1UL<<32));
    struct lines l = {0};
    struct ints times = {0};
    struct str time_line = str_strip_prefix(readline(&l), S_LIT("Time:"));
    parse_nums(time_line, &times, &a);
    struct ints dists = {0};
    struct str dist_line = str_strip_prefix(readline(&l), S_LIT("Distance:"));
    parse_nums(dist_line, &dists, &a);
    assert(times.len == dists.len);
    ssize_t prod = 1;
    for (ssize_t ridx = 0; ridx < times.len; ++ridx) {
        ssize_t ms = times.data[ridx];
        ssize_t record = dists.data[ridx];
        ssize_t opts = 0;
        for (ssize_t bms = 1; bms < ms; ++bms) {
            ssize_t dist = (ms - bms) * bms;
            if (dist > record) {
                ++opts;
            }
        }
        if (opts) {
            prod *= opts;
        }
    }
    printf("%zd\n", prod);
}
