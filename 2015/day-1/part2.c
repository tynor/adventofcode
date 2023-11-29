#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int8_t byte_lookup[256] = {
    ['('] = 1,
    [')'] = -1,
};

int main(void)
{
    uint8_t input_buf[1<<16];
    ssize_t floor = 0;
    ssize_t p = 0;
    for (;;) {
        ssize_t n = read(0, input_buf, sizeof(input_buf));
        if (!n) {
            break;
        }
        if (n == -1) {
            perror("read");
            return 1;
        }
        for (ssize_t i = 0; i != n; ++i) {
            ++p;
            floor += byte_lookup[input_buf[i]];
            if (floor < 0) {
                goto print;
            }
        }
    }
print:
    printf("%zd\n", p);
}
