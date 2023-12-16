cmake_minimum_required(VERSION 3.10)

project(d16 VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
add_executable(d16 ${SRC_FILES})
target_link_libraries(d16 AoCHelpers)
