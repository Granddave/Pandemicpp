cmake_minimum_required(VERSION 3.13)

set(SRC
    city_test.cpp
    main_test.cpp)

add_executable(pandemic-test ${SRC})
target_link_libraries(pandemic-test PRIVATE
    pandemic-lib
    catch)
