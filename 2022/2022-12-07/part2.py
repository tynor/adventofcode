from heapq import heappop, heappush
from sys import stdin


def main():
    root = {}
    p = [root]

    for line in stdin:
        line = line.rstrip()
        if line.startswith("$"):
            line = line[2:]
            if line.startswith("cd"):
                d = line[3:]
                if d == "/":
                    p = [root]
                elif d == "..":
                    p = p[:-1]
                else:
                    p.append(p[-1].setdefault(d, {}))
        elif line.startswith("dir"):
            _, _, name = line.partition(" ")
            p[-1][name] = {}
        else:
            size, _, name = line.partition(" ")
            p[-1][name] = int(size)

    sizes = []

    def rec_sum(n):
        total = 0
        for name, e in n.items():
            if isinstance(e, dict):
                total += rec_sum(e)
            else:
                total += e
        heappush(sizes, total)
        return total

    root_total = rec_sum(root)
    unused = TOTAL_SPACE - root_total
    needed = REQUIRED_FREE - unused
    while True:
        size = heappop(sizes)
        if size >= needed:
            print(size)
            break


TOTAL_KEY = "\x00TOTAL\x00"

TOTAL_SPACE = 70_000_000

REQUIRED_FREE = 30_000_000

if __name__ == "__main__":
    main()
