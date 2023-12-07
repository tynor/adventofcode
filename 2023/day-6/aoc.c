#include <assert.h>
#include <ctype.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct arena
{
    void *mem;
    void *p;
    void *end;
};

int arena_init(struct arena *a, ssize_t limit);

void *arena_push(struct arena *a, ssize_t size, ssize_t align);

#define arena_push_t(a, t) arena_push_array(a, t, 1)

#define arena_push_array(a, t, n) \
    (t *)arena_push((a), sizeof(t)*(n), alignof(t))

struct str
{
    uint8_t *data;
    ssize_t len;
};

#define S(s) (uint8_t *)s, sizeof(s)-1

#define S_LIT(s) (struct str){S(s)}

struct str str_cut(struct str s, ssize_t i);

struct str str_slice(struct str s, ssize_t b, ssize_t e);

uint64_t str_hash(struct str s);

bool str_eq(struct str a, struct str b);

struct str str_dup(struct str s, struct arena *a);

struct str next_word(struct str s);

ssize_t parse_int(struct str s, struct str *end);

struct lines
{
    /*
     * r points to the read head.
     */
    uint8_t *r;
    /*
     * w points to the write head.
     */
    uint8_t *w;
    uint8_t buf[1<<16];
    /*
     * true if EOF was read.
     */
    bool eof;
};

/*
 * readline reads a line from stdin and returns it as a string.
 */
struct str readline(struct lines *l);

ssize_t lines_fill(struct lines *l);

int arena_init(struct arena *a, ssize_t limit)
{
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANON | MAP_PRIVATE;
    void *mem = mmap(0, limit, prot, flags, -1, 0);
    if (mem == MAP_FAILED) {
        return -1;
    }
    *a = (struct arena){
        .mem = mem,
        .p = mem,
        .end = (uint8_t *)mem + limit,
    };
    return 0;
}

void *arena_push(struct arena *a, ssize_t size, ssize_t align)
{
    void *p = __builtin_align_up(a->p, align);
    a->p = (uint8_t *)p + size;
    memset(p, 0, size);
    return p;
}

bool str_has_prefix(struct str s, struct str prefix)
{
    return s.len >= prefix.len && !memcmp(s.data, prefix.data, prefix.len);
}

struct str str_strip_prefix(struct str s, struct str prefix)
{
    return str_has_prefix(s, prefix) ? str_cut(s, prefix.len) : s;
}

struct str str_cut(struct str s, ssize_t i)
{
    return str_slice(s, i, s.len);
}

struct str str_slice(struct str s, ssize_t b, ssize_t e)
{
    b = MAX(b, 0);
    e = MIN(e, s.len);
    if (b >= e) {
        return (struct str){0};
    }
    return (struct str){s.data + b, e - b};
}

struct str str_cut_after(struct str s, uint8_t c)
{
    for (ssize_t i = 0; i != s.len; ++i) {
        if (s.data[i] == c) {
            return str_cut(s, i+1);
        }
    }
    return (struct str){0, 0};
}

struct str str_trim_ws(struct str s)
{
    ssize_t i = 0;
    while (i < s.len && s.data[i] == ' ') {
        ++i;
    }
    return str_cut(s, i);
}

uint64_t str_hash(struct str s)
{
    uint64_t h = 0x100;
    for (ssize_t i = 0; i != s.len; ++i) {
        h ^= s.data[i];
        h *= 1111111111111111111U;
    }
    return h;
}

bool str_eq(struct str a, struct str b)
{
    return a.len == b.len && !memcmp(a.data, b.data, a.len);
}

struct str str_dup(struct str s, struct arena *a)
{
    uint8_t *data = arena_push_array(a, uint8_t, s.len);
    memcpy(data, s.data, s.len);
    return (struct str){data, s.len};
}

struct str next_word(struct str s)
{
    for (ssize_t i = 0; i != s.len; ++i) {
        if (s.data[i] == ' ') {
            return (struct str){s.data, i};
        }
    }
    return (struct str){0, 0};
}

ssize_t parse_int(struct str s, struct str *end)
{
    ssize_t i = 0;
    while (i != s.len && s.data[i] == ' ') {
        ++i;
    }
    ssize_t n = 0;
    for (; i != s.len; ++i) {
        if ('0' <= s.data[i] && s.data[i] <= '9') {
            n *= 10;
            n += s.data[i] - '0';
        } else {
            break;
        }
    }
    if (end) {
        *end = str_cut(s, i);
    }
    return n;
}

struct str readline(struct lines *l)
{
    uint8_t *r = l->r;
    ssize_t len = l->w - r;
    if (len) {
        uint8_t *p = memchr(r, '\n', len);
        if (p) {
            l->r = p + 1;
            return (struct str){r, l->r-r};
        }
    }

    for (;;) {
        ssize_t n = lines_fill(l);
        r = l->r;
        if (!n) {
            break;
        }
        uint8_t *p = memchr(r+len, '\n', l->w-r-len);
        if (p) {
            l->r = p + 1;
            len = l->r - r;
            break;
        }
        len += n;
    }
    return (struct str){r, len};
}

ssize_t lines_fill(struct lines *l)
{
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
    if (n == -1) {
        perror("read");
        exit(1);
    }
    l->w += n;
    return n;
}

#define push(s, a) \
    ((s)->len >= (s)->cap \
     ? grow(s, sizeof(*(s)->data), a), \
       (s)->data + (s)->len++ \
     : (s)->data + (s)->len++)

void grow(void *slice, ssize_t size, struct arena *a)
{
    struct {
        void *data;
        ssize_t len;
        ssize_t cap;
    } replica;
    memcpy(&replica, slice, sizeof(replica));

    replica.cap = replica.cap ? replica.cap : 1;
    ssize_t align = 16;
    void *data = arena_push(a, 2*size*replica.cap, align);
    replica.cap *= 2;
    if (replica.len) {
        memcpy(data, replica.data, size*replica.len);
    }
    replica.data = data;

    memcpy(slice, &replica, sizeof(replica));
}

uint64_t hash64(uint64_t x)
{
    x ^= x >> 32;
    x *= 0xd6e8feb86659fd93U;
    x ^= x >> 32;
    x *= 0xd6e8feb86659fd93U;
    x ^= x >> 32;
    return x;
}
