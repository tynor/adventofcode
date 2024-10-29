#!/bin/sh

set -u

CFLAGS="-std=c17 -g3 -pedantic -Wall -Wextra"
CFLAGS="$CFLAGS -Wconversion -Wdouble-promotion -Wno-unused-parameter"
CFLAGS="$CFLAGS -Wno-unused-function -Wno-sign-conversion"
CFLAGS="$CFLAGS -Wno-missing-field-initializers -Wno-unused-variable"
CFLAGS="$CFLAGS -Wno-string-conversion"
CFLAGS="$CFLAGS -fsanitize=address,undefined -fsanitize-trap"

echo "$CFLAGS"
