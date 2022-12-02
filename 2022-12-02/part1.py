from sys import stdin


def main():
    score = 0
    for line in stdin:
        op, _, my = line.rstrip().partition(" ")
        score += SCORES[op][my]
    print(score)


OP_ROCK = "A"
OP_PAPER = "B"
OP_SCISSORS = "C"

MY_ROCK = "X"
MY_PAPER = "Y"
MY_SCISSORS = "Z"

SCORES = {
    OP_ROCK: {
        MY_ROCK: 1 + 3,
        MY_PAPER: 2 + 6,
        MY_SCISSORS: 3 + 0,
    },
    OP_PAPER: {
        MY_ROCK: 1 + 0,
        MY_PAPER: 2 + 3,
        MY_SCISSORS: 3 + 6,
    },
    OP_SCISSORS: {
        MY_ROCK: 1 + 6,
        MY_PAPER: 2 + 0,
        MY_SCISSORS: 3 + 3,
    }
}


if __name__ == "__main__":
    main()
