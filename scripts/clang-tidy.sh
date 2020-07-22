#!/bin/sh
# clang-tidy.sh
#
# Run with --fix to apply the recommendations that clang-tidy identifies.

cd "$(dirname "$0")/.."

SRC_DIRS="src test"
FILES=$(find $SRC_DIRS -type f -regex ".*\.[ch]\(pp\)?$")

clang-tidy \
    $FILES \
    "$@"
