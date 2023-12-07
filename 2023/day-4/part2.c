#include "aoc.c"

struct intmap
{
    struct intmap *child[4];
    ssize_t key;
    ssize_t value;
};

ssize_t *lookup(struct intmap **m, ssize_t key, struct arena *a)
{
    for (uint64_t h = hash64((uint64_t)key); *m; h <<= 2) {
        if (key == (*m)->key) {
            return &(*m)->value;
        }
        m = &(*m)->child[h>>62];
    }
    if (!a) {
        static ssize_t zero = 0;
        return &zero;
    }
    *m = arena_push_t(a, struct intmap);
    (*m)->key = key;
    return &(*m)->value;
}

int main(void)
{
    struct arena perm = {0};
    assert(!arena_init(&perm, 1UL<<32));
    struct arena tmp = {0};
    assert(!arena_init(&tmp, 1UL<<32));
    struct intmap *cm = 0;
    ssize_t total_cards = 0;
    struct lines l = {0};
    for (;;) {
        struct str line = readline(&l);
        if (!line.len) {
            break;
        }
        struct arena tmp_a = tmp;
        struct intmap *winners = 0;
        line = str_strip_prefix(line, S_LIT("Card "));
        ssize_t round_idx = parse_int(line, &line);
        line = str_cut(line, 1);
        for (;;) {
            line = str_trim_ws(line);
            if (line.data[0] == '|') {
                break;
            }
            ssize_t n = parse_int(line, &line);
            *lookup(&winners, n, &tmp_a) = 1;
        }
        ssize_t copies = *lookup(&cm, round_idx, 0);
        ssize_t matches = 0;
        line = str_cut(line, 1);
        for (;;) {
            line = str_trim_ws(line);
            if (!line.len || line.data[0] == '\n') {
                break;
            }
            ssize_t n = parse_int(line, &line);
            matches += *lookup(&winners, n, 0);
        }
        ssize_t round_cards = copies + 1;
        total_cards += round_cards;
        for (ssize_t i = 1; i <= matches; ++i) {
            *lookup(&cm, round_idx+i, &perm) += round_cards;
        }
    }
    printf("%zd\n", total_cards);
}
