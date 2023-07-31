from sys import stdin


def main():
    x = 1
    cycle = 0

    def tick():
        nonlocal cycle

        draw()
        cycle += 1

    def draw():
        col = cycle % 40
        c = "."
        if x - 1 <= col <= x + 1:
            c = "#"
        end = ""
        if col == 39:
            end = "\n"
        print(c, end=end)

    for line in stdin:
        inst = line.rstrip()
        if inst == "noop":
            tick()
        else:
            _, _, v = inst.partition(" ")
            tick()
            tick()
            x += int(v)


if __name__ == "__main__":
    main()
