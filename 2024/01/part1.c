#include "common.c"

typedef struct
{
    I32 *data;
    Size len;
    Size cap;
} I32s;

static void sort_i32s(I32s v)
{
    for (I32 i = 1; i < v.len; i++) {
        for (I32 j = i; j > 0 && v.data[j-1] > v.data[j]; j--) {
            I32 tmp = v.data[j];
            v.data[j] = v.data[j-1];
            v.data[j-1] = tmp;
        }
    }
}

int main(void)
{
    Arena a = arena_new(100 * 1024 * 1024);

    Lines l = lines_new(&a, 4 * 1024);

    I32s left = (I32s){
        .data=new(&a, I32, 1024),
        .cap=1024,
    };
    I32s right = (I32s){
        .data=new(&a, I32, 1024),
        .cap=1024,
    };

    for (;;) {
        Str line = readline(&l);
        if (!line.data) {
            break;
        }
        I32 left_id = parse_i32(line, &line);
        line = trim_space(line);
        I32 right_id = parse_i32(line, 0);
        *push(&left, &a) = left_id;
        *push(&right, &a) = right_id;
    }

    sort_i32s(left);
    sort_i32s(right);

    I32 sum = 0;
    for (I32 i = 0; i < left.len; i++) {
        I32 d = left.data[i] - right.data[i];
        if (d < 0) {
            d *= -1;
        }
        sum += d;
    }
    printf("%d\n", sum);
}
