cmake_minimum_required(VERSION 3.13)

add_executable(pandemic-test
    city_test.cpp
    commondata_test.h
    disease_test.cpp
    main_test.cpp
    utils_test.cpp)
target_link_libraries(pandemic-test PRIVATE
    pandemic
    Catch2::Catch2
    spdlog::spdlog)
target_compile_definitions(pandemic-test PRIVATE LOG_OFF)
target_compile_options(pandemic-test PRIVATE ${COMPILER_WARNINGS})
set_target_properties(pandemic-test PROPERTIES CXX_STANDARD 20)

configure_file(
    ${CMAKE_SOURCE_DIR}/res/cities_data.txt
    ${CMAKE_CURRENT_BINARY_DIR}/cities_data.txt
    COPYONLY)
