cmake_minimum_required(VERSION 3.8)
project(openmp)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(SOURCE_FILES main.cpp)
add_executable(openmp ${SOURCE_FILES})