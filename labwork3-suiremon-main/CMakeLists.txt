cmake_minimum_required(VERSION 3.26)
project(labwork3_suiremon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
include_directories(.)

add_executable(labwork3_suiremon
        bin/main.cpp
        lib/Parser.cpp
        lib/Parser.h
        lib/Sandpile.cpp
        lib/Sandpile.h
        lib/Bmp.cpp
        lib/Bmp.h)
