cmake_minimum_required(VERSION 2.8)

# Project Declaration
project(SeamCarving CXX)

# Source Declaration
add_executable(SeamCarving
  main.cpp
  ImageLoader.cpp
)

set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++11")
