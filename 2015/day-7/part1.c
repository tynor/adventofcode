#include "aoc.c"

enum
{
    WIRE_VALUE,
    WIRE_AND,
    WIRE_OR,
    WIRE_NOT,
    WIRE_LSHIFT,
    WIRE_RSHIFT,
};

struct wire_input
{
    int32_t value;
    struct str wire;
};

struct wire_input parse_input(struct str s, struct str *end, struct arena *a)
{
    ssize_t i = 0;
    while (i < s.len && s.data[i] == ' ') {
        ++i;
    }
    if ('0' <= s.data[i] && s.data[i] <= '9') {
        int32_t value = (int32_t)s.data[i++] - '0';
        while (i < s.len && '0' <= s.data[i] && s.data[i] <= '9') {
            value *= 10;
            value += (int32_t)s.data[i++] - '0';
        }
        *end = str_cut(s, i);
        return (struct wire_input){value, {0}};
    }
    ssize_t start = i;
    while (i < s.len && 'a' <= s.data[i] && s.data[i] <= 'z') {
        ++i;
    }
    *end = str_cut(s, i);
    struct str w = str_slice(s, start, i);
    return (struct wire_input){-1, str_dup(w, a)};
}

struct wire
{
    int type;
    struct wire_input inputs[2];
};

struct wiremap
{
    struct wiremap *child[4];
    struct str key;
    struct wire value;
};

struct wire *getwire(struct wiremap **m, struct str key, struct arena *a)
{
    for (uint64_t h = str_hash(key); *m; h <<= 2) {
        if (str_eq(key, (*m)->key)) {
            return &(*m)->value;
        }
        m = &(*m)->child[h>>62];
    }
    if (!a) {
        return 0;
    }
    *m = arena_push_t(a, struct wiremap);
    (*m)->key = key;
    return &(*m)->value;
}

uint16_t eval_wire(struct str name, struct wiremap *m);

uint16_t eval_input(struct wire_input i, struct wiremap *m)
{
    if (i.value >= 0) {
        return (uint16_t)i.value;
    }
    return eval_wire(i.wire, m);
}

uint16_t eval_wire(struct str name, struct wiremap *m)
{
    struct wire *w = getwire(&m, name, 0);
    assert(w);
    uint16_t r = 0;
    uint16_t v0 = eval_input(w->inputs[0], m);
    if (w->type == WIRE_VALUE) {
        return v0;
    } else if (w->type == WIRE_NOT) {
        r = ~v0;
    } else {
        uint16_t v1 = eval_input(w->inputs[1], m);
        switch (w->type) {
        case WIRE_AND:
            r = v0 & v1;
            break;
        case WIRE_OR:
            r = v0 | v1;
            break;
        case WIRE_LSHIFT:
            r = v0 << v1;
            break;
        case WIRE_RSHIFT:
            r = v0 >> v1;
            break;
        }
    }
    *w = (struct wire){
        .type = WIRE_VALUE,
        .inputs = {{.value=r}},
    };
    return r;
}

struct str AND = S_LIT(" AND");
struct str OR = S_LIT(" OR");
struct str LSHIFT = S_LIT(" LSHIFT");
struct str RSHIFT = S_LIT(" RSHIFT");

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1L<<32));
    struct wiremap *m = 0;
    struct lines l = {0};
    for (;;) {
        struct str line = readline(&l);
        if (!line.data) {
            break;
        }
        struct wire w = {0};
        if (str_has_prefix(line, S_LIT("NOT"))) {
            w.type = WIRE_NOT;
            line = str_cut(line, sizeof("NOT ")-1);
            w.inputs[0] = parse_input(line, &line, &a);
        } else {
            w.inputs[0] = parse_input(line, &line, &a);
            if (str_has_prefix(line, S_LIT(" ->"))) {
                w.type = WIRE_VALUE;
            } else {
                if (str_has_prefix(line, AND)) {
                    w.type = WIRE_AND;
                    line = str_cut(line, AND.len);
                } else if (str_has_prefix(line, OR)) {
                    w.type = WIRE_OR;
                    line = str_cut(line, OR.len);
                } else if (str_has_prefix(line, LSHIFT)) {
                    w.type = WIRE_LSHIFT;
                    line = str_cut(line, LSHIFT.len);
                } else if (str_has_prefix(line, RSHIFT)) {
                    w.type = WIRE_RSHIFT;
                    line = str_cut(line, RSHIFT.len);
                } 
                w.inputs[1] = parse_input(line, &line, &a);
            }
        }
        ssize_t wire_idx = sizeof(" -> ")-1;
        struct str key = str_dup(str_cut(line, wire_idx), &a);
        struct wire *ww = getwire(&m, key, &a);
        *ww = w;
    }
    uint16_t v = eval_wire(S_LIT("a"), m);
    printf("%d\n", v);
}
