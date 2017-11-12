cmake_minimum_required(VERSION 3.8)
project(integral)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")


set(SOURCE_FILES main.cpp)
add_executable(integral ${SOURCE_FILES})