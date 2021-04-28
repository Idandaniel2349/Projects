cmake_minimum_required(VERSION 3.17)
project(OS1 C)

set(CMAKE_C_STANDARD 11)

add_library(OS1 library.c library.h pt.c)

add_executable(os os.c)