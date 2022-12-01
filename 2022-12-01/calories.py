from heapq import heappop, heappush
from itertools import repeat
from sys import stdin


def main():
    elves = []
    eof = False
    while not eof:
        elf_cals = 0
        while True:
            line = stdin.readline()
            if line == "\n":
                break
            elif line == "":
                eof = True
                break
            elf_cals += int(line.rstrip())
        heappush(elves, -elf_cals)
    total = 0
    for _ in repeat(None, 3):
        total += -heappop(elves)
    print(total)


if __name__ == "__main__":
    main()
