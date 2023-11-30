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
    return p;
}
