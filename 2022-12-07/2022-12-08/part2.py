from itertools import takewhile
from sys import stdin


def main():
    grid = stdin.readline().rstrip()
    width = len(grid)
    grid += "".join(line.rstrip() for line in stdin)
    height = len(grid) // width

    def left_edge(idx):
        return idx // width * width

    def right_edge(idx):
        return (idx + width - 1) // width * width

    highest = 0
    for y in range(1, height-1):
        for x in range(1, width-1):
            idx = y * width + x
            t = grid[idx]
            up = grid[idx-width:0:-width]
            for i, tt in enumerate(up, start=1):
                if tt >= t:
                    n_up = i
                    break
            else:
                n_up = len(up)
            right = grid[idx+1:right_edge(idx)]
            for i, tt in enumerate(right, start=1):
                if tt >= t:
                    n_right = i
                    break
            else:
                n_right = len(right)
            down = grid[idx+width::width]
            for i, tt in enumerate(down, start=1):
                if tt >= t:
                    n_down = i
                    break
            else:
                n_down = len(down)
            left = grid[idx-1:left_edge(idx)-1:-1]
            for i, tt in enumerate(left, start=1):
                if tt >= t:
                    n_left = i
                    break
            else:
                n_left = len(left)
            scenic_score = n_up * n_right * n_down * n_left
            highest = max(highest, scenic_score)

    print(highest)


if __name__ == "__main__":
    main()
