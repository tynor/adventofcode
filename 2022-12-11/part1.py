from collections import namedtuple
from dataclasses import dataclass
from itertools import repeat
from sys import stdin


def main():
    monkeys = []
    for line in stdin:
        if line.startswith("Monkey "):
            monkeys.append(parse_monkey(stdin))

    for _ in repeat(None, 20):
        for m in monkeys:
            items, m.items = m.items, []
            m.inspected += len(items)
            for item in items:
                n = m.op(item) // 3
                if n % m.test == 0:
                    idx = m.if_true
                else:
                    idx = m.if_false
                monkeys[idx].items.append(n)

    active = sorted(monkeys, key=lambda m: m.inspected, reverse=True)
    print(active[0].inspected * active[1].inspected)


def parse_monkey(f):
    line = f.readline().strip()
    assert line.startswith("Starting items:")
    _, _, items = line.partition(": ")
    items = [int(x) for x in items.split(", ")]
    line = f.readline().strip()
    assert line.startswith("Operation: ")
    _, _, op = line.partition(" = ")
    op = eval(f"lambda old: {op}")
    line = f.readline().strip()
    assert line.startswith("Test: ")
    _, _, test = line.partition(" by ")
    test = int(test)
    line = f.readline().strip()
    assert line.startswith("If true: ")
    _, _, if_true = line.partition(" monkey ")
    if_true = int(if_true)
    line = f.readline().strip()
    assert line.startswith("If false: ")
    _, _, if_false = line.partition(" monkey ")
    if_false = int(if_false)
    return Monkey(items, op, test, if_true, if_false)


@dataclass
class Monkey:
    items: list
    op: callable
    test: int
    if_true: int
    if_false: int
    inspected: int = 0


if __name__ == "__main__":
    main()
