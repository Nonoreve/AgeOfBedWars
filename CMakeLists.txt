cmake_minimum_required(VERSION 3.17)
project(AgeOfBedWars)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRCS    src/*.cpp src/*.c)
include_directories(includes)

add_executable(AgeOfBedWars ${SRCS} ${HEADERS})

find_library(YAML yaml-cpp libraries)
target_link_libraries(AgeOfBedWars ${YAML})