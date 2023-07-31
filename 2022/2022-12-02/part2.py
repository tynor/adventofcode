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

MY_LOSE = "X"
MY_DRAW = "Y"
MY_WIN = "Z"

ROCK_SCORE = 1
PAPER_SCORE = 2
SCISSORS_SCORE = 3

SCORES = {
    OP_ROCK: {
        MY_LOSE: 0 + SCISSORS_SCORE,
        MY_DRAW: 3 + ROCK_SCORE,
        MY_WIN: 6 + PAPER_SCORE,
    },
    OP_PAPER: {
        MY_LOSE: 0 + ROCK_SCORE,
        MY_DRAW: 3 + PAPER_SCORE,
        MY_WIN: 6 + SCISSORS_SCORE,
    },
    OP_SCISSORS: {
        MY_LOSE: 0 + PAPER_SCORE,
        MY_DRAW: 3 + SCISSORS_SCORE,
        MY_WIN: 6 + ROCK_SCORE,
    }
}


if __name__ == "__main__":
    main()
