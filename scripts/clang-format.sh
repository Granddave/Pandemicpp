#!/bin/bash

cd "$(dirname "$0")"

python3 external/run-clang-format.py \
    -r \
    ../src \
    ../test \
    -- -v

