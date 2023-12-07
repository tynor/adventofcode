#include "aoc.c"

struct intset
{
    struct intset *child[4];
    ssize_t key;
};

bool intset_member(struct intset **m, ssize_t key, struct arena *a)
{
    for (uint64_t h = hash64((uint64_t)key); *m; h <<= 2) {
        if (key == (*m)->key) {
            return true;
        }
        m = &(*m)->child[h>>62];
    }
    if (!a) {
        return false;
    }
    *m = arena_push_t(a, struct intset);
    (*m)->key = key;
    return false;
}

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1UL<<32));
    ssize_t sum = 0;
    struct lines l = {0};
    for (;;) {
        struct str line = readline(&l);
        if (!line.len) {
            break;
        }
        struct arena tmp = a;
        struct intset *winners = 0;
        line = str_cut_after(line, ':');
        for (;;) {
            line = str_trim_ws(line);
            if (line.data[0] == '|') {
                break;
            }
            ssize_t n = parse_int(line, &line);
            intset_member(&winners, n, &tmp);
        }
        ssize_t score = 0;
        line = str_cut(line, 1);
        for (;;) {
            line = str_trim_ws(line);
            if (!line.len) {
                break;
            }
            ssize_t n = parse_int(line, &line);
            if (intset_member(&winners, n, 0)) {
                score = score ? score*2 : 1;
            }
        }
        sum += score;
    }
    printf("%zd\n", sum);
}
