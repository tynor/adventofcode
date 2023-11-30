#include "aoc.h"

struct vec2
{
    int32_t x;
    int32_t y;
};

uint64_t vec2hash(struct vec2 v);

bool vec2eq(struct vec2 a, struct vec2 b);

struct vec2set
{
    struct vec2set *child[4];
    struct vec2 key;
};

bool visited(struct vec2set **m, struct vec2 key, struct arena *a);

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1UL<<32));
    ssize_t bufsz = 1 << 16;
    uint8_t *buf = arena_push_array(&a, uint8_t, bufsz);
    ssize_t houses = 0;
    struct vec2 pos = {0};
    struct vec2set *m = 0;
    houses += !visited(&m, pos, &a);
    for (;;) {
        ssize_t n = read(0, buf, bufsz);
        if (!n) {
            break;
        }
        if (n == -1) {
            perror("read");
            return 1;
        }
        for (ssize_t i = 0; i != n; ++i) {
            switch (buf[i]) {
            case '<':
                --pos.x;
                break;
            case '>':
                ++pos.x;
                break;
            case 'v':
                --pos.y;
                break;
            case '^':
                ++pos.y;
                break;
            }
            houses += !visited(&m, pos, &a);
        }
    }
    printf("%zd\n", houses);
}

uint64_t vec2hash(struct vec2 v)
{
    uint64_t i = ((uint64_t)v.x << 32) | (uint64_t)v.y;
    i ^= i >> 32;
    i *= 0xd6e8feb86659fd93U;
    i ^= i >> 32;
    i *= 0xd6e8feb86659fd93U;
    i ^= i >> 32;
    return i;
}

bool vec2eq(struct vec2 a, struct vec2 b)
{
    return a.x == b.x && a.y == b.y;
}

bool visited(struct vec2set **m, struct vec2 key, struct arena *a)
{
    for (uint64_t h = vec2hash(key); *m; h <<= 2) {
        if (vec2eq(key, (*m)->key)) {
            return true;
        }
        m = &(*m)->child[h>>62];
    }
    *m = arena_push_t(a, struct vec2set);
    (*m)->key = key;
    return false;
}

#include "aoc.c"
