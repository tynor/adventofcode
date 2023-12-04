#include "aoc.c"

bool symbol(uint8_t c)
{
    if ('0' <= c && c <= '9') {
        return false;
    }
    return c != '.' && c != '\n';
}

#define CLAMP(x, lo, hi) MIN(MAX(x, lo), hi)

bool valid_part(
        uint8_t *schema,
        ssize_t n,
        ssize_t stride,
        ssize_t start_idx,
        ssize_t end_idx)
{
    ssize_t u_i = CLAMP(start_idx - stride - 1, 0, n);
    ssize_t u_lim = CLAMP(end_idx - stride + 1, 0, n);
    for (; u_i < u_lim; ++u_i) {
        if (symbol(schema[u_i])) {
            return true;
        }
    }
    ssize_t d_i = CLAMP(start_idx + stride - 1, 0, n);
    ssize_t d_lim = CLAMP(end_idx + stride + 1, 0, n);
    for (; d_i < d_lim; ++d_i) {
        if (symbol(schema[d_i])) {
            return true;
        }
    }
    if (start_idx && symbol(schema[start_idx-1])) {
        return true;
    }
    if (end_idx < n && symbol(schema[end_idx])) {
        return true;
    }
    return false;
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
    fprintf(stderr, "grid cols: %zd\n", cols);
    ssize_t sum = 0;
    for (ssize_t i = 0; i != n;) {
        if (schema[i] < '0' || schema[i] > '9') {
            ++i;
            continue;
        }
        uint8_t *start = schema + i;
        uint8_t *end = 0;
        long part_num = strtol((char *)start, (char **)&end, 10);
        ssize_t len = end - start;
        if (valid_part(schema, n, stride, i, i+len)) {
            sum += part_num;
        }
        i += len;
    }
    printf("%zd\n", sum);
}
