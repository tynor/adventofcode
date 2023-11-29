#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define IS_DIGIT(b) ((b) >= '0' && (b) <= '9')

struct str
{
    char *data;
    ssize_t len;
};

struct str str_slice(struct str s, ssize_t start);

struct liner
{
    /*
     * r points to the read offset.
     */
    char *r;
    /*
     * w points to the write offset.
     */
    char *w;
    char buf[1<<16];
    /*
     * True if an EOF was read.
     */
    bool eof;
};

ssize_t liner_fill(struct liner *l);

int liner_line(struct liner *l, struct str *s);

ssize_t parse_int(struct str s, ssize_t *n);

struct dims
{
    ssize_t d0;
    ssize_t d1;
    ssize_t d2;
};

struct dims parse_dims(struct str line);

ssize_t dims_ribbon(struct dims ds);

int main(void)
{
    ssize_t total = 0;
    struct liner l = {0};
    while (!l.eof) {
        if (liner_fill(&l) == -1) {
            perror("read");
            return 1;
        }
        while (l.r != l.w) {
            struct str line;
            int r = liner_line(&l, &line);
            if (r) {
                break;
            }
            struct dims ds = parse_dims(line);
            total += dims_ribbon(ds);
        }
    }
    printf("%ld\n", total);
}

struct str str_slice(struct str s, ssize_t start)
{
    if (start > s.len) {
        start = s.len;
    }
    return (struct str){
        .data = s.data + start,
        .len = s.len - start,
    };
}

ssize_t liner_fill(struct liner *l)
{
    if (l->eof) {
        return 0;
    }
    if (l->w - l->r < 1024) {
        /*
         * The read and write heads are close, reset back to the
         * beginning of the buffer.
         * This also covers the initialized-to-zero case.
         */
        if (l->w != l->r) {
            memmove(l->buf, l->r, l->w-l->r);
        }
        l->r = l->w = l->buf;
    }
    ssize_t rem = sizeof(l->buf) - (l->w - l->buf);
    ssize_t n = read(0, l->w, rem);
    if (!n) {
        l->eof = true;
        return l->w - l->r;
    }
    if (n == -1) {
        return -1;
    }
    l->w += n;
    return l->w - l->r;
}

int liner_line(struct liner *l, struct str *s)
{
    char *p = memchr(l->r, '\n', l->w-l->r);
    if (!p) {
        return -1 - l->eof;
    }
    s->data = l->r;
    s->len = p - l->r;
    l->r = p + 1;
    return 0;
}

ssize_t parse_int(struct str s, ssize_t *n)
{
    ssize_t v = 0;
    char *p = s.data;
    char *end = p + s.len;
    while (p != end && IS_DIGIT(*p)) {
        v *= 10;
        v += *p - '0';
        ++p;
    }
    if (n) {
        *n = p - s.data;
    }
    return v;
}

struct dims parse_dims(struct str line)
{
    ssize_t n = 0;
    ssize_t d0 = parse_int(line, &n);
    if (n == line.len || line.data[n] != 'x') {
        fprintf(stderr, "malformed line\n");
        exit(1);
    }
    line = str_slice(line, n + 1);
    ssize_t d1 = parse_int(line, &n);
    if (n == line.len || line.data[n] != 'x') {
        fprintf(stderr, "malformed line\n");
        exit(1);
    }
    line = str_slice(line, n + 1);
    ssize_t d2 = parse_int(line, &n);
    if (n != line.len) {
        fprintf(stderr, "malformed line\n");
        exit(1);
    }
    return (struct dims){d0, d1, d2};
}

ssize_t dims_ribbon(struct dims ds)
{
    ssize_t p = (ds.d0 + ds.d1 + ds.d2 - MAX(ds.d0, MAX(ds.d1, ds.d2))) * 2;
    ssize_t a = ds.d0 * ds.d1 * ds.d2;
    return p + a;
}
