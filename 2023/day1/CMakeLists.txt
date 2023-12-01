cmake_minimum_required(VERSION 3.10)

project(Day1 VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
add_executable(Day1 ${SRC_FILES})
target_link_libraries(Day1 AoCHelpers)
