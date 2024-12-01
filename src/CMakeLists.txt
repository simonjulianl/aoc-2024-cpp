cmake_minimum_required(VERSION 3.31)
project(aoc2024 CXX)

set(CMAKE_C_COMPILER /usr/bin/gcc-14)
set(CMAKE_CXX_COMPILER /usr/bin/g++-14)
set(CMAKE_GENERATOR "Ninja")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field)

add_subdirectory(aoc)