cmake_minimum_required(VERSION 3.10)
project(AdventOfCode VERSION 2023.1)

IF(EXISTS /home/fds/build/gcc-11/lib64/)
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/home/fds/build/gcc-11/lib64/")
ENDIF()

OPTION(CMAKE_COMPILE_WARNING_AS_ERROR "Treat warnings as errors during compilation" ON)

add_subdirectory(helpers)
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
