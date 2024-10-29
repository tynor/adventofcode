#!/bin/sh

year=$1
day=$(printf "%02d" "$2")

mkdir -p "$year/$day"
curl \
    -L \
    -b "session=$AOC_SESS" \
    -o "$year/$day/input.txt" \
    "https://adventofcode.com/$year/day/$((10#$day))/input"

if [ ! -f "$year/$day/common.c" ]; then
    cp common/common.c "$year/$day/"
fi
