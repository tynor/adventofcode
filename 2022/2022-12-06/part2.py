from itertools import islice
from sys import stdin


def main():
    run_len = 14
    it = chars(stdin)
    data = "".join(islice(it, run_len))
    if len(set(data)) == len(data):
        print(4)
        return
    for i, c in enumerate(it, start=run_len + 1):
        data = data[1:] + c
        if len(set(data)) == len(data):
            print(i)
            break


def chars(f):
    while (data := f.read(4096)) != "":
        yield from data


if __name__ == "__main__":
    main()
