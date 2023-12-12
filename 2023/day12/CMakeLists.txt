cmake_minimum_required(VERSION 3.10)

project(d12 VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
add_executable(d12 ${SRC_FILES})
target_link_libraries(d12 AoCHelpers)
