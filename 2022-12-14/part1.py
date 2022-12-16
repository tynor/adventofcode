from array import array
from collections import namedtuple
from itertools import repeat
from sys import stdin, stdout


def main():
    min_x, max_x = 500, 500
    min_y, max_y = 0, 0
    line_coords = []
    for line in stdin:
        coords = [to_coord(x) for x in line.rstrip().split(" -> ")]
        line_coords.append(coords)
        for x, y in coords:
            min_x = min(min_x, x)
            max_x = max(max_x, x)
            min_y = min(min_y, y)
            max_y = max(max_y, y)

    height = max_y - min_y + 1
    width = max_x - min_x + 1

    grid = array("b", repeat(0, times=height*width))

    def togrid(v):
        return (v.y - min_y) * width + (v.x - min_x)

    def nextstep(drop):
        x, y = drop
        y += 1
        v = Vec2(x, y)
        if not grid[togrid(v)]:
            return v
        v = Vec2(x - 1, y)
        if not grid[togrid(v)]:
            return v
        v = Vec2(x + 1, y)
        if not grid[togrid(v)]:
            return v
        return None

    water_source = togrid(Vec2(500, 0))

    def drawgrid(drop=None):
        for row in range(0, height*width, width):
            for idx in range(row, row+width):
                if idx == drop:
                    c = "\x1b[1mo\x1b[0m"  # ]]
                elif idx == water_source:
                    c = "+"
                else:
                    v = grid[idx]
                    if v == 1:
                        c = "#"
                    elif v == 2:
                        c = "o"
                    else:
                        c = "."
                stdout.write(c)
            stdout.write("\n")

    for lc in line_coords:
        for i in range(len(lc)-1):
            f = lc[i]
            t = lc[i+1]
            if f.x == t.x:
                if f.y > t.y:
                    didx = -width
                else:
                    didx = width
            elif f.y == t.y:
                if f.x > t.x:
                    didx = -1
                else:
                    didx = 1
            else:
                raise RuntimeError("not a straight line")
            s = togrid(f)
            e = togrid(t)
            for idx in range(s, e+didx, didx):
                grid[idx] = 1

    n = 0
    while True:
        drop = Vec2(500, 0)
        while drop.y < max_y:
            drop1 = nextstep(drop)
            if drop1 is None:
                break
            drop = drop1
        if drop.y >= max_y:
            break
        grid[togrid(drop)] = 2
        n += 1
    print(n)


def to_coord(x):
    x, _, y = x.partition(",")
    return Vec2(int(x), int(y))


Vec2 = namedtuple('Vec2', "x y")

if __name__ == "__main__":
    main()
