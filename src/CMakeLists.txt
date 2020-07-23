cmake_minimum_required(VERSION 3.13)

# Lib
add_library(pandemic STATIC
    board.cpp
    board.h
    card.cpp
    card.h
    city.cpp
    city.h
    disease.h
    game.cpp
    game.h
    instrumentor.h
    logging.cpp
    logging.h
    player.cpp
    player.h
    utils.cpp
    utils.h)
target_include_directories(pandemic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pandemic PUBLIC spdlog::spdlog)
target_compile_options(pandemic PRIVATE ${COMPILER_WARNINGS})

# Application
add_executable(pandemic-app main.cpp)
target_link_libraries(pandemic-app PRIVATE pandemic)
target_compile_options(pandemic-app PRIVATE ${COMPILER_WARNINGS})

configure_file(
    ${CMAKE_SOURCE_DIR}/res/cities_data.txt
    ${CMAKE_CURRENT_BINARY_DIR}/cities_data.txt
    COPYONLY)
