from functools import cmp_to_key
from itertools import chain, zip_longest
import json
from sys import stdin


def main():
    div1 = [[2]]
    div2 = [[6]]
    ps = sorted(
        chain(packets(stdin), [div1, div2]),
        key=cmp_to_key(lambda l, r: -right_order(l, r)),
    )

    div1_idx = None
    div2_idx = None
    for i, p in enumerate(ps, start=1):
        if p == div1:
            div1_idx = i
        elif p == div2:
            div2_idx = i
            break
    print(div1_idx * div2_idx)


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
