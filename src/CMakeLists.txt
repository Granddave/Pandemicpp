cmake_minimum_required(VERSION 3.13)

# Lib
add_library(pandemic-lib STATIC
    board.cpp
    board.h
    card.cpp
    card.h
    city.cpp
    city.h
    disease.h
    game.cpp
    game.h
    logging.cpp
    logging.h
    player.cpp
    player.h
    utils.cpp
    utils.h)
target_include_directories(pandemic-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pandemic-lib PUBLIC
    spdlog::spdlog)

# Application
add_executable(pandemic main.cpp)
target_link_libraries(pandemic PRIVATE
    pandemic-lib)

configure_file(
    ${CMAKE_SOURCE_DIR}/res/cities_data.txt
    ${CMAKE_CURRENT_BINARY_DIR}/cities_data.txt
    COPYONLY)
