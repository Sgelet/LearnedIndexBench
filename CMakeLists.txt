cmake_minimum_required(VERSION 3.22)
project(LearnedIndexBench)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(LearnedIndexBench main.cpp)
