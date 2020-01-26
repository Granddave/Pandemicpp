cmake_minimum_required(VERSION 3.13)

set(SRC
    board.cpp
    card.cpp
    city.cpp
    main.cpp
    player.cpp)

set(HDR
    board.h
    card.h
    city.h
    disease.h
    player.h)

add_executable(pandemic ${SRC} ${HDR})

configure_file(
    ${CMAKE_SOURCE_DIR}/res/cities_data.txt
    ${CMAKE_CURRENT_BINARY_DIR}/cities_data.txt
    COPYONLY)
