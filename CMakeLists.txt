cmake_minimum_required(VERSION 3.26)
project(LearnedIndexBench)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

find_package(CGAL)

add_executable(LearnedIndexBench main.cpp)
target_link_libraries(LearnedIndexBench CGAL::CGAL)
