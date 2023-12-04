#include "aoc.c"

ssize_t extract_num(uint8_t *schema, ssize_t i)
{
    while (i > 0 && isdigit(schema[i-1])) {
        --i;
    }
    return strtol((char *)schema+i, 0, 10);
}

ssize_t gear_ratio(
        uint8_t *schema,
        ssize_t n,
        ssize_t stride,
        ssize_t gear_idx)
{
    ssize_t p0 = -1;
    ssize_t p1 = -1;

#define ADD_PART_NUM(n) \
    do { \
        if (p0 == -1) { \
            p0 = (n); \
        } else if (p1 == -1) { \
            p1 = (n); \
        } else { \
            return -1; \
        } \
    } while (0)

    if (gear_idx > stride) {
        ssize_t i = gear_idx - stride - 1;
        if (i >= 0 && isdigit(schema[i])) {
            ADD_PART_NUM(extract_num(schema, i));
            if (!isdigit(schema[i+1]) && isdigit(schema[i+2])) {
                ADD_PART_NUM(extract_num(schema, i+2));
            }
        } else if (isdigit(schema[i+1])) {
            ADD_PART_NUM(extract_num(schema, i+1));
        } else if (isdigit(schema[i+2])) {
            ADD_PART_NUM(extract_num(schema, i+2));
        }
    }
    if (gear_idx + stride < n) {
        ssize_t i = gear_idx + stride - 1;
        if (i >= 0 && isdigit(schema[i])) {
            ADD_PART_NUM(extract_num(schema, i));
            if (!isdigit(schema[i+1]) && isdigit(schema[i+2])) {
                ADD_PART_NUM(extract_num(schema, i+2));
            }
        } else if (isdigit(schema[i+1])) {
            ADD_PART_NUM(extract_num(schema, i+1));
        } else if (isdigit(schema[i+2])) {
            ADD_PART_NUM(extract_num(schema, i+2));
        }
    }
    if (gear_idx && isdigit(schema[gear_idx-1])) {
        ADD_PART_NUM(extract_num(schema, gear_idx-1));
    }
    if (gear_idx+1 < n && isdigit(schema[gear_idx+1])) {
        ADD_PART_NUM(strtol((char *)schema+gear_idx+1, 0, 10));
    }

    return p0 * p1;

#undef ADD_PART_NUM
}

int main(void)
{
    struct arena a = {0};
    assert(!arena_init(&a, 1UL<<32));
    ssize_t schema_cap = 32000;
    uint8_t *schema = arena_push_array(&a, uint8_t, schema_cap);
    ssize_t n = read(0, schema, schema_cap);
    if (n == -1) {
        perror("read");
        return 1;
    }
    uint8_t *nl = memchr(schema, '\n', n);
    ssize_t cols = nl - schema;
    ssize_t stride = cols + 1;
    ssize_t sum = 0;
    ssize_t i = 0;
    while (i < n) {
        uint8_t *gear = memchr(schema+i, '*', n-i);
        if (!gear) {
            break;
        }
        ssize_t gear_idx = gear - schema;
        ssize_t ratio = gear_ratio(schema, n, stride, gear_idx);
        if (ratio > 0) {
            sum += ratio;
        }
        i = gear_idx + 1;
    }
    printf("%zd\n", sum);
}
