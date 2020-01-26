cmake_minimum_required(VERSION 3.13)

set(SRC
    board.cpp
    card.cpp
    city.cpp
    player.cpp)

set(HDR
    board.h
    card.h
    city.h
    disease.h
    player.h)

add_library(pandemic-lib STATIC ${SRC} ${HDR})
target_include_directories(pandemic-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(pandemic main.cpp)
target_link_libraries(pandemic PRIVATE pandemic-lib)

configure_file(
    ${CMAKE_SOURCE_DIR}/res/cities_data.txt
    ${CMAKE_CURRENT_BINARY_DIR}/cities_data.txt
    COPYONLY)
