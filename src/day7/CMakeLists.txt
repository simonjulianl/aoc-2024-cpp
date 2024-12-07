add_library(input)
target_sources(input PUBLIC FILE_SET CXX_MODULES FILES input.cpp)
add_library(part_one)
target_sources(part_one PUBLIC FILE_SET CXX_MODULES FILES part_one.cpp)
add_library(part_two)
target_sources(part_two PUBLIC FILE_SET CXX_MODULES FILES part_two.cpp)

add_executable(aoc2024 main.cpp)
target_link_libraries(aoc2024 input part_one part_two)
