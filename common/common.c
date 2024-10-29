#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define assert(e) if (!(e)) { __builtin_trap(); }

typedef int32_t I32;

typedef int32_t B32;
typedef unsigned char Byte;
typedef ptrdiff_t Size;
typedef uintptr_t UPtr;

#define new(a, t, n) (t *)make(a, sizeof(t), _Alignof(t), n)

typedef struct
{
    Byte *beg;
    Byte *end;
} Arena;

static Arena arena_new(Size sz)
{
    Byte *beg = malloc(sz);
    return (Arena){beg, beg+sz};
}

static Byte *make(Arena *a, Size sz, Size align, Size n)
{
    assert(n >= 0);
    Size pad = (UPtr)a->end & (align - 1);
    assert(n < (a->end - a->beg - pad)/sz);
    return memset(a->end -= sz*n + pad, 0, sz*n);
}

static Byte *makefront(Arena *a, Size sz, Size align, Size n)
{
    Size pad = -(UPtr)a->beg & (align - 1);
    assert(n < (a->end - a->beg - pad)/sz);
    Byte *p = a->beg + pad;
    a->beg += pad + sz*n;
    return memset(p, 0, sz*n);
}

static void *clone(Arena *a, void *data, Size len, Size sz, Size align)
{
    void *p = makefront(a, sz, align, len);
    return memcpy(p, data, len*sz);
}

#define push(s, arena) \
    ((s)->len >= (s)->cap \
     ? grow(s, sizeof(*(s)->data), arena), (s)->data + (s)->len++ \
     : (s)->data + (s)->len++)

static void grow(void *slice, Size sz, Arena *a)
{
    struct
    {
        void *data;
        Size len;
        Size cap;
    } replica;
    memcpy(&replica, slice, sizeof(replica));
    Size align = 16;

    if (!replica.cap) {
        replica.cap = 1;
        replica.data = makefront(a, sz, align, 1);
        return;
    }

    if ((Byte *)replica.data + replica.cap * sz != a->beg) {
        replica.data = clone(a, replica.data, replica.cap, sz, align);
    }
    Size delta = replica.cap * 0.5;
    makefront(a, sz, align, delta);
    replica.cap += delta;

    memcpy(slice, &replica, sizeof(replica));
}

#define S(s) (Str){(const Byte *)s, sizeof(s)-1}

typedef struct
{
    const Byte *data;
    Size len;
} Str;

static B32 str_eq(Str a, Str b)
{
    return a.len == b.len && !memcmp(a.data, b.data, a.len);
}

typedef struct
{
    Byte *buf;
    Size cap;
    B32 eof;

    Byte *p;
    Byte *end;
} Scanner;

static Scanner *scanner_new(Arena *a, Size cap)
{
    Scanner *s = new(a, Scanner, 1);
    s->buf = new(a, Byte, cap);
    s->cap = cap;
    s->end = s->p = s->buf;
    return s;
}

static void fill(Scanner *s)
{
    if (s->eof) {
        return;
    }
    Size len = s->end - s->p;
    if (len && s->p != s->buf) {
        memmove(s->buf, s->p, len);
        s->p = s->buf;
        s->end = s->buf + len;
    }
    Size rem = s->cap - len;
    Size n = read(0, s->end, rem);
    if (n == -1) {
        perror("read");
        exit(1);
    }
    if (!n) {
        s->eof = 1;
        return;
    }
    s->end += n;
}

static B32 b_isws(Byte b)
{
    return b == '\n' || b == ' ';
}

static B32 b_isdigit(Byte b)
{
    return '0' <= b && b <= '9';
}

static B32 b_isword(Byte b)
{
    return b_isdigit(b) || 'a' <= b && b <= 'z' || 'A' <= b && b <= 'Z';
}

static B32 skip_ws(Scanner *s)
{
    for (;;) {
        if (s->p != s->end) {
            if (!b_isws(*s->p)) {
                break;
            }
            s->p++;
        } else if (s->eof) {
            return 0;
        } else {
            fill(s);
        }
    }
    return 1;
}

static Str read_token(Scanner *s)
{
    if (!skip_ws(s)) {
        return (Str){0};
    }
    Byte *data = s->p;
    for (;;) {
        while (s->p != s->end) {
            if (!b_isword(*s->p)) {
                return (Str){data, s->p - data};
            }
            s->p++;
        }
        if (s->eof) {
            return (Str){data, s->p - data};
        }
        fill(s);
    }
}

static I32 read_int(Scanner *s)
{
    if (!skip_ws(s)) {
        return -1;
    }
    I32 v = 0;
    for (;;) {
        while (s->p != s->end) {
            if (!b_isdigit(*s->p)) {
                if (!v) {
                    return -1;
                }
                return v;
            }
            v *= 10;
            v += *s->p++ - '0';
        }
        if (s->eof) {
            return v;
        }
        fill(s);
    }
}
