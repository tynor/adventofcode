#include "common.c"

typedef struct i32_map I32Map;

struct i32_map
{
    I32Map *child[4];
    I32 key;
    I32 value;
};

I32 *upsert(I32Map **m, I32 key, Arena *perm)
{
    for (U64 h = hash64(key); *m; h <<= 2) {
        if (key == (*m)->key) {
            return &(*m)->value;
        }
        m = &(*m)->child[h>>62];
    }
    if (!perm) {
        return 0;
    }
    *m = new(perm, I32Map, 1);
    (*m)->key = key;
    return &(*m)->value;
}

typedef struct
{
    I32 *data;
    Size len;
    Size cap;
} I32s;

int main(void)
{
    Arena a = arena_new(100 * 1024 * 1024);

    Lines l = lines_new(&a, 4 * 1024);

    I32s left = (I32s){
        .data=new(&a, I32, 1024),
        .cap=1024,
    };

    I32Map *right = 0;

    for (;;) {
        Str line = readline(&l);
        if (!line.data) {
            break;
        }
        I32 left_id = parse_i32(line, &line);
        *push(&left, &a) = left_id;
        line = trim_space(line);
        I32 right_id = parse_i32(line, 0);
        (*upsert(&right, right_id, &a))++;
    }

    I32 sum = 0;
    for (I32 i = 0; i < left.len; i++) {
        I32 v = left.data[i];
        v *= *upsert(&right, v, &a);
        sum += v;
    }
    printf("%d\n", sum);
}
