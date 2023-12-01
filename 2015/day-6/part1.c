#include "aoc.h"

#define X 1000

#define Y 1000

int8_t grid[X*Y];

#define COORD(x, y) ((x)*X+(y))

struct vec2
{
    ssize_t x;
    ssize_t y;
};

#define OP_ON 0
#define OP_OFF 1
#define OP_TOG 2

void setlights(struct vec2 p0, struct vec2 p1, int op)
{
    ssize_t x0 = MIN(p0.x, p1.x);
    ssize_t x1 = MAX(p0.x, p1.x);
    ssize_t y0 = MIN(p0.y, p1.y);
    ssize_t y1 = MAX(p0.y, p1.y);
    for (ssize_t i = x0; i <= x1; ++i) {
        for (ssize_t j = y0; j <= y1; ++j) {
            switch (op) {
            case OP_ON:
                grid[COORD(i, j)] = 1;
                break;
            case OP_OFF:
                grid[COORD(i, j)] = 0;
                break;
            case OP_TOG:
                grid[COORD(i, j)] ^= 1;
                break;
            }
        }
    }
}

#define S(x) x, sizeof(x)-1

int main(void)
{
    struct lines l = {0};
    for (;;) {
        struct str line = readline(&l);
        if (!line.data) {
            break;
        }
        int op = -1;
        if (!memcmp(line.data, S("toggle"))) {
            op = OP_TOG;
            line = str_cut(line, sizeof("toggle ")-1);
        } else if (!memcmp(line.data, S("turn on"))) {
            op = OP_ON;
            line = str_cut(line, sizeof("turn on ")-1);
        } else if (!memcmp(line.data, S("turn off"))) {
            op = OP_OFF;
            line = str_cut(line, sizeof("turn off ")-1);
        }
        assert(op != -1);
        ssize_t x0 = parse_int(line, &line);
        assert(x0 != -1 && line.len && line.data[0] == ',');
        line = str_cut(line, 1);
        ssize_t y0 = parse_int(line, &line);
        assert(y0 != -1 && !memcmp(line.data, S(" through ")));
        line = str_cut(line, sizeof(" through ")-1);
        ssize_t x1 = parse_int(line, &line);
        assert(x1 != -1 && line.len && line.data[0] == ',');
        line = str_cut(line, 1);
        ssize_t y1 = parse_int(line, 0);
        assert(y1 != -1);

        struct vec2 p0 = {x0, y0};
        struct vec2 p1 = {x1, y1};
        setlights(p0, p1, op);
    }
    ssize_t lit = 0;
    for (ssize_t x = 0; x < X; ++x) {
        for (ssize_t y = 0; y < Y; ++y) {
            lit += grid[COORD(x, y)];
        }
    }
    printf("%zd\n", lit);
}

#include "aoc.c"
