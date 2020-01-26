cmake_minimum_required(VERSION 3.13)

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

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()
