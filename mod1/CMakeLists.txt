cmake_minimum_required(VERSION 2.8)
# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
add_library(mod1 STATIC
  func1.cpp
  func2.cpp
  logger.cpp
)
