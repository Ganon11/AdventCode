cmake_minimum_required(VERSION 3.10)

project(d1 VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
add_executable(d1 ${SRC_FILES})
target_link_libraries(d1 AoCHelpers)
target_compile_options(d1 PRIVATE -Wall -Wextra -Wpedantic -Werror)
