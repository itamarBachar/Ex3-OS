cmake_minimum_required(VERSION 3.20)
project(Ex3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Ex3 main.cpp QueueBounded.cpp QueueBounded.h QueueUnbounded.cpp QueueUnbounded.cpp)
TARGET_LINK_LIBRARIES(Ex3 pthread)