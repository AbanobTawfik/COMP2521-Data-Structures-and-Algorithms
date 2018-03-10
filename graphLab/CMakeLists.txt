cmake_minimum_required(VERSION 3.8)
project(maps)

set(CMAKE_C_STANDARD 99)

set(P1_SOURCE_FILES  places.c places.h nnn.c)
add_executable(p1 ${P1_SOURCE_FILES})

set(CONN_SOURCE_FILES conn.c map.c Map.h places.c places.h)
add_executable(conn ${CONN_SOURCE_FILES})

set(EURO_SOURCE_FILES euro.c places.c map.c Map.h  places.h)
add_executable(euro ${EURO_SOURCE_FILES})