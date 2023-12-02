#include "aoc.c"

struct colorset
{
    ssize_t red;
    ssize_t green;
    ssize_t blue;
};

struct colorset round_power(struct str s)
{
    struct colorset cs = {0};
    while (s.len) {
        ssize_t n = parse_int(s, &s);
        if (str_has_prefix(s, S_LIT(" red"))) {
            cs.red = MAX(cs.red, n);
            s = str_cut(s, sizeof(" red")-1);
        } else if (str_has_prefix(s, S_LIT(" green"))) {
            cs.green = MAX(cs.green, n);
            s = str_cut(s, sizeof(" green")-1);
        } else if (str_has_prefix(s, S_LIT(" blue"))) {
            cs.blue = MAX(cs.blue, n);
            s = str_cut(s, sizeof(" blue")-1);
        }
        s = str_strip_prefix(s, S_LIT(", "));
    }
    return cs;
}

ssize_t game_power(struct str line)
{
    struct colorset cs = {0};
    uint8_t *c = memchr(line.data, ':', line.len);
    line = str_cut(line, c+1-line.data);
    for (;;) {
        uint8_t *d = memchr(line.data, ';', line.len);
        if (!d) {
            struct colorset rcs = round_power(line);
            cs.red = MAX(cs.red, rcs.red);
            cs.green = MAX(cs.green, rcs.green);
            cs.blue = MAX(cs.blue, rcs.blue);
            break;
        }
        ssize_t idx = d - line.data;
        struct str round = str_slice(line, 0, idx);
        struct colorset rcs = round_power(round);
        cs.red = MAX(cs.red, rcs.red);
        cs.green = MAX(cs.green, rcs.green);
        cs.blue = MAX(cs.blue, rcs.blue);
        line = str_cut(line, idx+1);
    }
    return cs.red * cs.green * cs.blue;
}

int main(void)
{
    struct lines l = {0};
    ssize_t sum = 0;
    for (;;) {
        struct str line = readline(&l);
        if (!line.data) {
            break;
        }
        sum += game_power(line);
    }
    printf("%zd\n", sum);
}
