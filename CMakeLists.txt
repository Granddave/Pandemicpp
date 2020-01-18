cmake_minimum_required(VERSION 2.8)

project(pandemic)

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

add_executable(${PROJECT_NAME} ${SRC} ${HDR})
