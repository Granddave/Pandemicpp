#!/bin/sh

cd "$(dirname "$0")"

run-clang-tidy \
    -header-filter="-external" \
    -j4 \
    ../src/**.cpp \
    ../test/**.cpp
