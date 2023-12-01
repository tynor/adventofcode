#include "aoc.h"

bool nice_str(struct str s, struct arena a);

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1<<16));
    struct lines l = {0};
    ssize_t nnice = 0;
    for (;;) {
        struct str line = readline(&l);
        if (!line.data) {
            break;
        }
        nnice += nice_str(line, a);
    }
    printf("%zd\n", nnice);
}

struct pairmap
{
    struct pairmap *child[4];
    uint16_t key;
    ssize_t start;
};

ssize_t *pairseen(struct pairmap **m, uint16_t key, struct arena *a)
{
    for (uint16_t h = key; *m; h <<= 2) {
        if (key == (*m)->key) {
            return &(*m)->start;
        }
        m = &(*m)->child[h>>14];
    }
    *m = arena_push_t(a, struct pairmap);
    (*m)->key = key;
    ssize_t *v = &(*m)->start;
    *v = -1;
    return v;
}

bool nice_str(struct str s, struct arena a)
{
    struct pairmap *m = 0;
    ssize_t npairs = 0;
    ssize_t ntrips = 0;
    for (ssize_t i = 0; i < s.len-1; ++i) {
        uint16_t key = ((uint16_t)s.data[i] << 8) | (uint16_t)s.data[i+1];
        ssize_t *start = pairseen(&m, key, &a);
        if (*start == -1) {
            *start = i;
        } else {
            npairs += *start + 1 < i;
        }
        if (i < s.len-2) {
            ntrips += s.data[i] == s.data[i+2];
        }
    }
    return npairs && ntrips;
}

#include "aoc.c"
