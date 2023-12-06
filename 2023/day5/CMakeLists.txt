cmake_minimum_required(VERSION 3.10)

project(d5 VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
add_executable(d5 ${SRC_FILES})
target_link_libraries(d5 AoCHelpers)
target_compile_options(d5 PRIVATE -Wall -Wextra -Wpedantic -Werror)
