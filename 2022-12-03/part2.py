from itertools import count
from string import ascii_letters
from sys import stdin


def main():
    total = 0
    for e1, e2, e3 in zip(*([(l.rstrip() for l in stdin)] * 3)):
        shared = set(e1) & set(e2) & set(e3)
        total += PRIO[shared.pop()]
    print(total)


PRIO = dict(zip(ascii_letters, count(1)))


if __name__ == "__main__":
    main()
