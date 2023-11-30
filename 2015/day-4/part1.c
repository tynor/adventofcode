#include "aoc.h"

#include "md5.h"

int main(void)
{
    char buf[1024];
    ssize_t inputLen = read(0, buf, sizeof(buf));
    if (inputLen == -1) {
        perror("read");
        return 1;
    }
    while (buf[inputLen-1] == '\n') {
        --inputLen;
    }
    buf[inputLen] = 0;
    ssize_t attempt = 1;
    char nbuf[100];
    for (;;) {
        ssize_t ndigit = 0;
        ssize_t i = attempt;
        while (i) {
            ssize_t dig = i % 10;
            i /= 10;
            nbuf[ndigit++] = '0' + (char)dig;
        }
        ssize_t trialLen = inputLen;
        while (ndigit) {
            buf[trialLen++] = nbuf[--ndigit];
        }
        buf[trialLen] = 0;
        uint8_t digest[16];
        md5String(buf, digest);
        if (!digest[0] && !digest[1] && !(digest[2] >> 4)) {
            break;
        }
        ++attempt;
    }
    printf("%zd\n", attempt);
}

#include "aoc.c"
#include "md5.c"
