cmake_minimum_required(VERSION 3.10)

project(d3 VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
add_executable(d3 ${SRC_FILES})
target_link_libraries(d3 AoCHelpers)
target_compile_options(d3 PRIVATE -Wall -Wextra -Wpedantic -Werror)
