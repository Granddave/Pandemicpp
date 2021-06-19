#!/bin/bash

cd "$(dirname "$0")/.."

SRC_DIRS="src"

cppcheck \
    --enable=all \
    $SRC_DIRS
