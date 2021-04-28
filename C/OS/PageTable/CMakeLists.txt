cmake_minimum_required(VERSION 3.17)
project(PageTable C)

set(CMAKE_C_STANDARD 11)

add_executable(PageTable os.c os.h pt.c)