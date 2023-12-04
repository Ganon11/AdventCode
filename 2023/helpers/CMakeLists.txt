SET(CMAKE_CXX_STANDARD 20)
file(GLOB SRC_FILES *.cpp)
add_library(AoCHelpers ${SRC_FILES})
target_include_directories(AoCHelpers PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)