from itertools import count
from string import ascii_letters
from sys import stdin


def main():
    total = 0
    for line in stdin:
        rucksack = line.rstrip()
        c1 = rucksack[:len(rucksack)//2]
        c2 = rucksack[len(rucksack)//2:]
        shared = set(c1) & set(c2)
        total += sum(PRIO[item] for item in shared)
    print(total)


PRIO = dict(zip(ascii_letters, count(1)))


if __name__ == "__main__":
    main()
