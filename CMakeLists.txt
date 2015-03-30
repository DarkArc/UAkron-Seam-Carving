cmake_minimum_required(VERSION 2.8)

# Project Declaration
project(Percolation CXX)

# Source Declaration
add_executable(Percolation
  main.cpp
)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
