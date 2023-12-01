void lines_fill(struct lines *l);

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

struct str str_cut(struct str s, ssize_t i)
{
    i = MIN(s.len, i);
    if (i == s.len) {
        return (struct str){0};
    }
    return (struct str){
        .data = s.data + i,
        .len = s.len - i,
    };
}

ssize_t parse_int(struct str s, struct str *end)
{
    ssize_t n = 0;
    for (ssize_t i = 0; i != s.len; ++i) {
        if ('0' <= s.data[i] && s.data[i] <= '9') {
            n *= 10;
            n += s.data[i] - '0';
        } else {
            if (end) {
                *end = str_cut(s, i);
            }
            break;
        }
    }
    return n;
}

struct str readline(struct lines *l)
{
    if (l->r == l->w) {
        if (l->eof) {
            return (struct str){0};
        }
        lines_fill(l);
        if (l->r == l->w && l->eof) {
            return (struct str){0};
        }
    }
    for (;;) {
        uint8_t *p = memchr(l->r, '\n', l->w-l->r);
        if (!p) {
            if (l->eof) {
                return (struct str){
                    .data = l->r,
                    .len = l->w - l->r,
                };
            }
            assert(l->w - l->r < (ssize_t)sizeof(l->buf));
            lines_fill(l);
            continue;
        }
        struct str s = {
            .data = l->r,
            .len = p - l->r,
        };
        l->r = p + 1;
        return s;
    }
}

void lines_fill(struct lines *l)
{
    if (l->eof) {
        return;
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
        return;
    }
    if (n == -1) {
        perror("read");
        exit(1);
    }
    l->w += n;
}
