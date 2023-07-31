from sys import stdin


def main():
    n = 0
    for line in stdin:
        e1, _, e2 = line.rstrip().partition(",")
        e1_start_s, _, e1_end_s = e1.partition("-")
        e1_start = int(e1_start_s)
        e1_end = int(e1_end_s)
        e2_start_s, _, e2_end_s = e2.partition("-")
        e2_start = int(e2_start_s)
        e2_end = int(e2_end_s)

        if e2_start <= e1_start <= e2_end or e2_start <= e1_end <= e2_end:
            n += 1
        elif e1_start <= e2_start <= e1_end or e1_start <= e2_end <= e1_end:
            n += 1
    print(n)


if __name__ == "__main__":
    main()
