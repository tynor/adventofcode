#!/bin/sh

set -exuo pipefail

srcdir="$AOC_YEAR/day-$AOC_DAY"
input="$srcdir/input.txt"
example="${2:-}"

run() {
    mkdir -p .build
    prog="./.build/$AOC_YEAR-$AOC_DAY-$1"
    clang -o "$prog" \
        -Wall -Wextra -pedantic \
        -std=c2x \
        -I./common \
        -fsanitize=address,undefined \
        -O2 -g3 \
        "$srcdir/part$1.c"
    if [ -n "$example" ]; then
        "$prog" <<<"$example"
    else
        "$prog" <"$input"
    fi
}

if [ "$1" = init ]; then
    if [ ! -d "$srcdir" ]; then
        mkdir -p "$srcdir"
    fi
    if [ ! -f "$input" ]; then
        curl -o "$input" --cookie "session=$AOC_SESS" \
            "https://adventofcode.com/$AOC_YEAR/day/$AOC_DAY/input"
    fi
    touch "$srcdir/part1.c"
elif [ "$1" = run1 ]; then
    run 1
elif [ "$1" = run2 ]; then
    run 2
fi
