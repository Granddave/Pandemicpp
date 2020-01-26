cmake_minimum_required(VERSION 3.13)

project(pandemic
    VERSION 0.1
    DESCRIPTION "Pandemic in C++"
    LANGUAGES CXX
)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(src)
