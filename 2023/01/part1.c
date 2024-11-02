#include "common.c"

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
            Byte b = l.data[i];
            if (!b_isdigit(b)) {
                continue;
            }
            Size v = b - '0';
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
