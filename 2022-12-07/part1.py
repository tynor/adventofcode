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

    total_sum = 0

    def rec_sum(n):
        nonlocal total_sum

        total = 0
        for name, e in n.items():
            if isinstance(e, dict):
                total += rec_sum(e)
            else:
                total += e
        if total <= 100_000:
            total_sum += total
        return total

    rec_sum(root)
    print(total_sum)


if __name__ == "__main__":
    main()
