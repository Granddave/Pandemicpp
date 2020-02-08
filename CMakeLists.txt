cmake_minimum_required(VERSION 3.13)

project(pandemic
    VERSION 0.1
    DESCRIPTION "Pandemic in C++"
    LANGUAGES CXX)

set(COMPILER_WARNINGS -Werror -Wall -Wextra)

add_subdirectory(external/Catch2)
add_subdirectory(external/spdlog)
add_subdirectory(src)
add_subdirectory(test)
