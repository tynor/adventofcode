from itertools import repeat
import re
from sys import stdin


def main():
    stacks = []

    for line in stdin:
        if line.startswith(" 1"):
            break
        crate = 0
        while line:
            seg, line = line[:4], line[4:]
            letter = seg[1:2]
            if letter == " ":
                crate += 1
                continue
            while len(stacks) < crate + 1:
                stacks.append([])
            stacks[crate].append(letter)
            crate += 1
    for stack in stacks:
        stack.reverse()

    next(stdin)

    for line in stdin:
        m = INST_RE.match(line)
        n = int(m.group("n"))
        src = stacks[int(m.group("src")) - 1]
        dest = stacks[int(m.group("dest")) - 1]
        for _ in repeat(None, n):
            crate = src.pop()
            dest.append(crate)

    print("".join(stack[-1] for stack in stacks))


INST_RE = re.compile(r"move (?P<n>\d+) from (?P<src>\d+) to (?P<dest>\d+)")



if __name__ == "__main__":
    main()
