/* ex: ft=c:
 */

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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
