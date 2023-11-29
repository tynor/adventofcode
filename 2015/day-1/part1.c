#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int8_t byte_lookup[256] = {
    ['('] = 1,
    [')'] = -1,
};

ssize_t popcnt(uint64_t n);
ssize_t countset(uint8x16_t p0, uint8x16_t p1, uint8x16_t p2, uint8x16_t p3);

int main(void)
{
    uint8_t input_buf[1<<16];
    uint8x16_t l_paren_mask = vdupq_n_u8('(');
    uint8x16_t r_paren_mask = vdupq_n_u8(')');
    ssize_t floor = 0;
    for (;;) {
        ssize_t n = read(0, input_buf, sizeof(input_buf));
        if (!n) {
            break;
        }
        if (n == -1) {
            perror("read");
            return 1;
        }
        ssize_t l_paren_count = 0;
        ssize_t r_paren_count = 0;
        ssize_t i = 0;
        while (i + 64 <= n) {
            uint8x16_t data0 = vld1q_u8(&input_buf[i]);
            uint8x16_t data1 = vld1q_u8(&input_buf[i+16]);
            uint8x16_t data2 = vld1q_u8(&input_buf[i+32]);
            uint8x16_t data3 = vld1q_u8(&input_buf[i+48]);

            uint8x16_t l0 = vceqq_u8(l_paren_mask, data0);
            uint8x16_t l1 = vceqq_u8(l_paren_mask, data1);
            uint8x16_t l2 = vceqq_u8(l_paren_mask, data2);
            uint8x16_t l3 = vceqq_u8(l_paren_mask, data3);
            l_paren_count += countset(l0, l1, l2, l3);

            uint8x16_t r0 = vceqq_u8(r_paren_mask, data0);
            uint8x16_t r1 = vceqq_u8(r_paren_mask, data1);
            uint8x16_t r2 = vceqq_u8(r_paren_mask, data2);
            uint8x16_t r3 = vceqq_u8(r_paren_mask, data3);
            r_paren_count += countset(r0, r1, r2, r3);

            i += 64;
        }
        floor += l_paren_count - r_paren_count;
        while (i < n) {
            floor += byte_lookup[input_buf[i]];
            ++i;
        }
    }
    printf("%zd\n", floor);
}

ssize_t popcnt(uint64_t n)
{
    ssize_t c = 0;
    for (; n; n >>= 1) {
        c += n & 1;
    }
    return c;
}

/*
 * Adapted from https://branchfree.org/2019/04/01/fitting-my-head-through-the-arm-holes-or-two-sequences-to-substitute-for-the-missing-pmovmskb-instruction-on-arm-neon/
 */
ssize_t countset(uint8x16_t p0, uint8x16_t p1, uint8x16_t p2, uint8x16_t p3)
{
    uint8x16_t bitmask1 = {
        0x01, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0x10,
        0x01, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0x10,
    };
    uint8x16_t bitmask2 = {
        0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x02, 0x20,
        0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x02, 0x20,
    };
    uint8x16_t bitmask3 = {
        0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40,
        0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40,
    };
    uint8x16_t bitmask4 = {
        0x08, 0x80, 0x08, 0x80, 0x08, 0x80, 0x08, 0x80,
        0x08, 0x80, 0x08, 0x80, 0x08, 0x80, 0x08, 0x80,
    };

    uint8x16_t t0 = vandq_u8(p0, bitmask1);
    uint8x16_t t1 = vbslq_u8(bitmask2, p1, t0);
    uint8x16_t t2 = vbslq_u8(bitmask3, p2, t1);
    uint8x16_t tmp = vbslq_u8(bitmask4, p3, t2);
    uint8x16_t sum = vpaddq_u8(tmp, tmp);
    uint64_t mask = vgetq_lane_u64(vreinterpretq_u64_u8(sum), 0);
    return popcnt(mask);
}
