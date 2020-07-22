#!/bin/bash

cd "$(dirname "$0")/.."

SRC_DIRS="src test"
FILES=$(find $SRC_DIRS -type f -regex ".*\.[ch]\(pp\)?$")

clang-format \
    $FILES \
    -i
