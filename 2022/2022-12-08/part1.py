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

    # edges are all visible
    visible = width * 2 + (height - 2) * 2
    for y in range(1, height-1):
        for x in range(1, width-1):
            idx = y * width + x
            t = grid[idx]
            seen_up = all(t > tt for tt in grid[idx-width:0:-width])
            seen_right = all(t > tt
                             for tt
                             in grid[idx+1:right_edge(idx)])
            seen_down = all(t > tt for tt in grid[idx+width::width])
            seen_left = all(t > tt
                            for tt
                            in grid[idx-1:left_edge(idx)-1:-1])
            if seen_up or seen_right or seen_down or seen_left:
                visible += 1

    print(visible)


if __name__ == "__main__":
    main()
