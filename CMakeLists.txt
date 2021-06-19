cmake_minimum_required(VERSION 3.13)

project(pandemic
    VERSION 0.1
    DESCRIPTION "Pandemic in C++"
    LANGUAGES CXX)

set(COMPILER_WARNINGS
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wuseless-cast
)

add_subdirectory(external/Catch2)
add_subdirectory(external/spdlog)
add_subdirectory(src)
add_subdirectory(test)
