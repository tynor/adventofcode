from sys import stdin


def main():
    x = 1
    cycle = 0
    result = 0

    def tick():
        nonlocal cycle
        nonlocal result

        cycle += 1
        if cycle in SIGNALS:
            result += cycle * x

    for line in stdin:
        inst = line.rstrip()
        if inst == "noop":
            tick()
        else:
            _, _, v = inst.partition(" ")
            tick()
            tick()
            x += int(v)

    print(result)


SIGNALS = {20, 60, 100, 140, 180, 220}

if __name__ == "__main__":
    main()
