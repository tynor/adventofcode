from itertools import islice, zip_longest
import json
from sys import stdin


def main():
    packet_pairs = []
    it = packets(stdin)
    while (batch := tuple(islice(it, 2))):
        packet_pairs.append(batch)

    n = 0
    for i, (l, r) in enumerate(packet_pairs, start=1):
        if right_order(l, r) > 0:
            n += i
    print(n)


def right_order(l, r):
    if isinstance(l, int) and isinstance(r, int):
        return r - l
    if isinstance(l, int):
        l = [l]
    elif isinstance(r, int):
        r = [r]
    for li, ri in zip_longest(l, r):
        if li is None:
            return 1
        if ri is None:
            return -1
        if (c := right_order(li, ri)):
            return c
    return 0


def packets(f):
    for line in f:
        if line == "\n":
            continue
        yield json.loads(line)


if __name__ == "__main__":
    main()
