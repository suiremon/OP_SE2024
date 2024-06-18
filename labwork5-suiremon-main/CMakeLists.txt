cmake_minimum_required(VERSION 3.26)
project(labwork5_suiremon)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
add_subdirectory(lib)
add_subdirectory(bin)
enable_testing()
add_subdirectory(tests)