#include "aoc.c"

struct ints
{
    ssize_t *data;
    ssize_t len;
    ssize_t cap;
};

struct seg
{
    ssize_t lo;
    ssize_t hi;
    ssize_t offset;
};

struct segmap
{
    struct seg *data;
    ssize_t len;
    ssize_t cap;
};

void sortsegs(struct segmap m)
{
    for (ssize_t i = 1; i < m.len; ++i) {
        for (ssize_t j = i; j && m.data[j-1].lo > m.data[j].lo; --j) {
            struct seg tmp = m.data[j];
            m.data[j] = m.data[j-1];
            m.data[j-1] = tmp;
        }
    }
}

void addseg(
        struct segmap *m,
        ssize_t dst_start,
        ssize_t src_start,
        ssize_t len,
        struct arena *a)
{
    *push(m, a) = (struct seg){
        .lo = src_start,
        .hi = src_start + len,
        .offset = dst_start - src_start,
    };
    sortsegs(*m);
}

ssize_t getval(struct segmap sm, ssize_t v)
{
    ssize_t l = 0;
    ssize_t r = sm.len - 1;
    while (l <= r) {
        ssize_t m = (l + r) / 2;
        if (sm.data[m].lo <= v && v < sm.data[m].hi) {
            return v + sm.data[m].offset;
        }
        if (sm.data[m].lo < v) {
            l = m + 1;
        } else if (sm.data[m].hi >= v) {
            r = m - 1;
        }
    }
    return v;
}

enum
{
    SEED_TO_SOIL,
    SOIL_TO_FERT,
    FERT_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMP,
    TEMP_TO_HUMID,
    HUMID_TO_LOC,

    N_MAPS,
};

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1UL<<32));
    struct ints seeds = {0};
    struct segmap mapseq[N_MAPS] = {{0}};
    struct segmap *cur = 0;
    struct lines l = {0};
    for (;;) {
        struct str line = readline(&l);
        if (!line.len) {
            break;
        }
        if (str_has_prefix(line, S_LIT("seeds: "))) {
            line = str_strip_prefix(line, S_LIT("seeds: "));
            for (;;) {
                ssize_t seed = parse_int(line, &line);
                if (!seed) {
                    break;
                }
                *push(&seeds, &a) = seed;
            }
            assert(str_eq(readline(&l), S_LIT("\n")));
        } else if (str_has_prefix(line, S_LIT("seed-to-soil"))) {
            cur = &mapseq[SEED_TO_SOIL];
        } else if (str_has_prefix(line, S_LIT("soil-to-fertilizer"))) {
            cur = &mapseq[SOIL_TO_FERT];
        } else if (str_has_prefix(line, S_LIT("fertilizer-to-water"))) {
            cur = &mapseq[FERT_TO_WATER];
        } else if (str_has_prefix(line, S_LIT("water-to-light"))) {
            cur = &mapseq[WATER_TO_LIGHT];
        } else if (str_has_prefix(line, S_LIT("light-to-temperature"))) {
            cur = &mapseq[LIGHT_TO_TEMP];
        } else if (str_has_prefix(line, S_LIT("temperature-to-humidity"))) {
            cur = &mapseq[TEMP_TO_HUMID];
        } else if (str_has_prefix(line, S_LIT("humidity-to-location"))) {
            cur = &mapseq[HUMID_TO_LOC];
        } else if (!str_eq(line, S_LIT("\n"))) {
            size_t dst_start = parse_int(line, &line);
            size_t src_start = parse_int(line, &line);
            size_t len = parse_int(line, &line);
            addseg(cur, dst_start, src_start, len, &a);
        }
    }
    ssize_t min_loc = -1;
    for (ssize_t i = 0; i < seeds.len; ++i) {
        ssize_t n = seeds.data[i];
        for (ssize_t m = 0; m < N_MAPS; ++m) {
            n = getval(mapseq[m], n);
        }
        if (min_loc == -1 || n < min_loc) {
            min_loc = n;
        }
    }
    printf("%zd\n", min_loc);
}
