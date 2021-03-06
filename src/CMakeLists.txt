cmake_minimum_required(VERSION 2.8)
# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake

find_package(Boost COMPONENTS thread system log log_setup REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions("-Wall -std=c++1y")
add_definitions("-DBOOST_LOG_DYN_LINK")

macro(logged_executable name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ARGN})
endmacro(logged_executable)

logged_executable(trivial)
logged_executable(set_filter)
logged_executable(logger)
logged_executable(channel)

add_subdirectory(mod1)

logged_executable(client mod1)
