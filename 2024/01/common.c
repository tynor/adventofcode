#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define assert(e) if (!(e)) { __builtin_trap(); }

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef int32_t I32;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int32_t B32;
typedef unsigned char Byte;
typedef ptrdiff_t Size;
typedef uintptr_t UPtr;

U64 hash64(U64 x)
{
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9U;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebU;
    x ^= x >> 31;
    return x;
}
#define new(a, t, n) (t *)make(a, sizeof(t), _Alignof(t), n)

typedef struct
{
    Byte *beg;
    Byte *end;
} Arena;

static Arena arena_new(Size sz)
{
    I32 prot = PROT_READ | PROT_WRITE;
    I32 flags = MAP_ANON | MAP_PRIVATE;
    Byte *beg = mmap(0, sz, prot, flags, -1, 0);
    if (beg == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
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
    Size delta = replica.cap / 2;
    makefront(a, sz, align, delta);
    replica.cap += delta;

    memcpy(slice, &replica, sizeof(replica));
}

#define S(s) (Str){(Byte *)s, sizeof(s)-1}

typedef struct
{
    Byte *data;
    Size len;
} Str;

static B32 str_eq(Str a, Str b)
{
    return a.len == b.len && !memcmp(a.data, b.data, a.len);
}

static B32 startswith(Str a, Str prefix)
{
    return prefix.len <= a.len && !memcmp(a.data, prefix.data, prefix.len);
}

static Str lslice(Str s, Size i)
{
    Byte *p = s.data + MIN(i, s.len);
    Byte *end = s.data + s.len;
    return (Str){p, end - p};
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
    return b_isdigit(b) || ('a' <= b && b <= 'z') || ('A' <= b && b <= 'Z');
}

static Str trim_space(Str s)
{
    Byte *p = s.data;
    Byte *end = s.data + s.len;
    while (p != end && *p == ' ') {
        p++;
    }
    return (Str){p, end - p};
}

static I32 parse_i32(Str s, Str *rem)
{
    I32 n = 0;
    Byte *p = s.data;
    Byte *end = s.data + s.len;
    while (p != end && b_isdigit(*p)) {
        n *= 10;
        n += *p++ - '0';
    }
    if (rem) {
        rem->data = p;
        rem->len = end - p;
    }
    return n;
}

typedef struct
{
    Byte *buf;
    Size cap;
    Byte *r;
    Byte *w;
    B32 eof;
} Lines;

static Lines lines_new(Arena *a, Size cap)
{
    Byte *buf = new(a, Byte, cap);
    return (Lines){
        .buf=buf,
        .cap=cap,
        .r=buf,
        .w=buf,
    };
}

static Size fill(Lines *l)
{
    if (l->eof) {
        return 0;
    }

    Size len = l->w - l->r;
    if (len < 1024) {
        if (l->w != l->r) {
            memmove(l->buf, l->r, len);
        }
        l->r = l->buf;
        l->w = l->buf + len;
    }
    Size rem = l->cap - (l->w - l->buf);
    Size n = read(0, l->w, rem);
    if (n == -1) {
        perror("read");
        exit(1);
    }
    l->w += n;
    l->eof = !n;
    return n;
}

static Str readline(Lines *l)
{
    Size len = 0;
    for (;;) {
        while (l->r + len != l->w) {
            if (l->r[len] == '\n') {
                Byte *data = l->r;
                l->r += len + 1;
                return (Str){data, len};
            }
            len++;
        }
        if (l->eof) {
            if (!len) {
                return (Str){0};
            }
            Byte *data = l->r;
            l->r += len;
            return (Str){data, len};
        }
        fill(l);
    }
}
