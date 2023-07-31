from itertools import repeat
from sys import stdin


def main():
    rope = [(0, 0)] * 10
    positions = {rope[-1]}

    def vec2_add(a, d):
        ax, ay = a
        dx, dy = d
        return ax + dx, ay + dy

    def vec2_sub(a, d):
        ax, ay = a
        dx, dy = d
        return ax - dx, ay - dy

    for line in stdin:
        d, _, n = line.rstrip().partition(" ")
        if d == "U":
            md = (0, 1)
        elif d == "R":
            md = (1, 0)
        elif d == "L":
            md = (-1, 0)
        elif d == "D":
            md = (0, -1)

        for _ in repeat(None, int(n)):
            rope[0] = vec2_add(rope[0], md)
            for i in range(len(rope)-1):
                a, b = rope[i], rope[i+1]
                v = vec2_sub(b, a)
                dv = VECS.get(v, None)
                if dv is None:
                    break
                rope[i+1] = vec2_add(b, dv)
            positions.add(rope[-1])

    print(len(positions))


# maps (dx, dy) to a movement vector
VECS = {
    # same column
    (0, -2): (0, 1),
    (0, 2): (0, -1),
    # same row
    (-2, 0): (1, 0),
    (2, 0): (-1, 0),
    # column behind
    (-1, -2): (1, 1),
    (-2, -2): (1, 1),
    (-1, 2): (1, -1),
    (-2, 2): (1, -1),
    # column ahead
    (1, -2): (-1, 1),
    (2, -2): (-1, 1),
    (1, 2): (-1, -1),
    (2, 2): (-1, -1),
    # row below
    (-2, -1): (1, 1),
    (-2, -2): (1, 1),
    (2, -1): (-1, 1),
    (2, -2): (-1, 1),
    # row above
    (-2, 1): (1, -1),
    (-2, 2): (1, -1),
    (2, 1): (-1, -1),
    (2, 2): (-1, -1),
}


if __name__ == "__main__":
    main()
