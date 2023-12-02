#include "aoc.c"

enum
{
    RED = 12,
    GREEN = 13,
    BLUE = 14,
};

bool round_possible(struct str s)
{
    while (s.len) {
        ssize_t n = parse_int(s, &s);
        if (str_has_prefix(s, S_LIT(" red"))) {
            if (n > RED) {
                return false;
            }
            s = str_cut(s, sizeof(" RED")-1);
        } else if (str_has_prefix(s, S_LIT(" green"))) {
            if (n > GREEN) {
                return false;
            }
            s = str_cut(s, sizeof(" GREEN")-1);
        } else if (str_has_prefix(s, S_LIT(" blue"))) {
            if (n > BLUE) {
                return false;
            }
            s = str_cut(s, sizeof(" BLUE")-1);
        }
        s = str_strip_prefix(s, S_LIT(", "));
    }
    return true;
}

ssize_t game_possible(struct str line)
{
    line = str_cut(line, sizeof("Game ")-1);
    ssize_t id = parse_int(line, &line);
    line = str_cut(line, 1);
    for (;;) {
        uint8_t *d = memchr(line.data, ';', line.len);
        if (!d) {
            if (!round_possible(line)) {
                return 0;
            }
            break;
        }
        ssize_t idx = d - line.data;
        struct str round = str_slice(line, 0, idx);
        if (!round_possible(round)) {
            return 0;
        }
        line = str_cut(line, idx+1);
    }
    return id;
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
        sum += game_possible(line);
    }
    printf("%zd\n", sum);
}
