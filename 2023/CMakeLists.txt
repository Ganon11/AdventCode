cmake_minimum_required(VERSION 3.10)

project(AdventOfCode VERSION 2023.1)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/home/fds/build/gcc-11/lib64/")

add_subdirectory(helpers)
add_subdirectory(day1)
