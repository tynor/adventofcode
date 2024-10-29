#!/bin/sh

year=$1
day=$(printf "%02d" "$2")
part=$3

mkdir -p .build

prog=.build/$year-$day-$part

clang $(./flags.sh) -o "$prog" "$year/$day/part$part.c"

exec "$prog"
