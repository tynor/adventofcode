from itertools import repeat
from sys import stdin


def main():
    headx, heady = (0, 0)
    tailx, taily = (0, 0)
    positions = {(tailx, taily)}

    def touching():
        return (
            headx - 1 <= tailx <= headx + 1 and
            heady - 1 <= taily <= heady + 1
        )

    for line in stdin:
        d, _, n = line.rstrip().partition(" ")
        if d == "U":
            for _ in repeat(None, int(n)):
                heady += 1
                if not touching():
                    tailx, taily = headx, heady - 1
                    positions.add((tailx, taily))
        elif d == "R":
            for _ in repeat(None, int(n)):
                headx += 1
                if not touching():
                    tailx, taily = headx - 1, heady
                    positions.add((tailx, taily))
        elif d == "D":
            for _ in repeat(None, int(n)):
                heady -= 1
                if not touching():
                    tailx, taily = headx, heady + 1
                    positions.add((tailx, taily))
        elif d == "L":
            for _ in repeat(None, int(n)):
                headx -= 1
                if not touching():
                    tailx, taily = headx + 1, heady
                    positions.add((tailx, taily))

    print(len(positions))


if __name__ == "__main__":
    main()
