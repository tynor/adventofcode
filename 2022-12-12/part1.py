from array import array
from collections import defaultdict, namedtuple
from sys import maxsize, stdin, stdout


def main():
    a = array('b')
    a.extend(ord(c) for c in stdin.readline().rstrip())
    width = len(a)
    for line in stdin:
        a.extend(ord(c) for c in line.rstrip())
    height = len(a) // width
    start = -1
    end = -1
    nodes = []
    for y in range(height):
        for x in range(width):
            i = y * width + x
            c = a[i]
            if c == ord("S"):
                start = i
                c = ord("a")
            elif c == ord("E"):
                end = i
                c = ord("z")
            edges = []
            up = i - width
            if y > 0 and can_step(c, a[up]):
                edges.append(up)
            down = i + width
            if y < height - 1 and can_step(c, a[down]):
                edges.append(down)
            left = i - 1
            if x > 0 and can_step(c, a[left]):
                edges.append(left)
            right = i + 1
            if x < width - 1 and can_step(c, a[right]):
                edges.append(right)
            nodes.append(Node(c, edges))
    assert end != -1 and start != -1

    def h(nodes, idx):
        return 0

    path = a_star(nodes, start, end, h)
    print(len(path) - 1)


def can_step(f, t):
    if t == ord("S"):
        t = ord("a")
    elif t == ord("E"):
        t = ord("z")
    return t - f <= 1


def draw(f, a, width, height, seen):
    seen = set(seen)
    for y in range(height):
        for x in range(width):
            idx = y * width + x
            s = chr(a[idx])
            if idx in seen:
                s = bold(s)
            f.write(s)
        f.write("\n")


def bold(c):
    return f"\x1b[1m{c}\x1b[0m"


def idx_to_coord(idx, width):
    return idx // width, idx % width


def a_star(nodes, start, end, h):
    open_set = {start}
    came_from = {}
    g_score = defaultdict(lambda: maxsize)
    g_score[start] = 0
    f_score = defaultdict(lambda: maxsize)
    f_score[start] = h(nodes, start)

    while open_set:
        current = min(open_set, key=lambda i: f_score[i])
        open_set.remove(current)
        if current == end:
            return reconstruct_path(came_from, current)

        for edge in nodes[current].edges:
            tg_score = g_score[current] + 1
            if tg_score < g_score[edge]:
                came_from[edge] = current
                g_score[edge] = tg_score
                f_score[edge] = tg_score + h(nodes, edge)
                open_set.add(edge)

    raise ValueError("no path to end")


def reconstruct_path(came_from, current):
    total_path = [current]
    while (current := came_from.get(current, None)) is not None:
        total_path.append(current)
    total_path.reverse()
    return total_path


Node = namedtuple("Node", "score edges")


if __name__ == "__main__":
    main()
