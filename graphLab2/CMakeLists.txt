cmake_minimum_required(VERSION 3.8)
project(graph)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES travel.c Graph.c Graph.h Queue.c Queue.h Item.h ha30_dist.txt ha30_name.txt)
add_executable(graph ${SOURCE_FILES})