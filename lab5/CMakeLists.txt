cmake_minimum_required(VERSION 3.3)
project(lab5)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_FILES implementations.h implementations.cpp main.cpp)
add_executable(lab5 ${SOURCE_FILES})