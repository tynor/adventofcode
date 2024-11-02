#include "common.c"

static Size parse_digit(Str s)
{
    if (s.len < 3) {
        return -1;
    }
    switch (s.data[0]) {
    case 'e':
    {
        if (startswith(s, S("eight"))) {
            return 8;
        }
    } break;
    case 'f':
    {
        if (startswith(s, S("five"))) {
            return 5;
        }
        if (startswith(s, S("four"))) {
            return 4;
        }
    } break;
    case 'n':
    {
        if (startswith(s, S("nine"))) {
            return 9;
        }
    } break;
    case 'o':
    {
        if (startswith(s, S("one"))) {
            return 1;
        }
    } break;
    case 's':
    {
        if (startswith(s, S("seven"))) {
            return 7;
        }
        if (startswith(s, S("six"))) {
            return 6;
        }
    } break;
    case 't':
    {
        if (startswith(s, S("three"))) {
            return 3;
        }
        if (startswith(s, S("two"))) {
            return 2;
        }
    } break;
    }
    return -1;
}

int main(void)
{
    Arena a = arena_new(1024 * 1024);
    Lines ls = lines_new(&a, 5000);
    Size sum = 0;
    for (;;) {
        Str l = readline(&ls);
        if (!l.data) {
            break;
        }
        Size first = -1;
        Size last = -1;
        for (Size i = 0; i < l.len; i++) {
            Size v = b_isdigit(l.data[i])
                ? l.data[i] - '0'
                : parse_digit(lslice(l, i));
            if (v == -1) {
                continue;
            }
            if (first == -1) {
                first = v;
            }
            last = v;
        }
        if (first == -1) {
            continue;
        }
        Size n = (first * 10) + last;
        sum += n;
    }
    printf("%zd\n", sum);
}
