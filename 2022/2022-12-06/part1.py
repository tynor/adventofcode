from itertools import islice
from sys import stdin


def main():
    it = chars(stdin)
    data = "".join(islice(it, 4))
    if len(set(data)) == len(data):
        print(4)
        return
    for i, c in enumerate(it, start=5):
        data = data[1:] + c
        if len(set(data)) == len(data):
            print(i)
            break


def chars(f):
    while (data := f.read(4096)) != "":
        yield from data


if __name__ == "__main__":
    main()
