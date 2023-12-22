cmake_minimum_required(VERSION 3.26)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(image_processor
        src/Header.cpp
        src/Header.h
        src/Main.cpp
        src/Pixel.cpp
        src/Pixel.h)
