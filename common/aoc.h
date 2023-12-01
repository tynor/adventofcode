/* ex: ft=c:
 */

#include <assert.h>
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
