cmake_minimum_required(VERSION 3.18)

add_library(threadpool threadpool.cpp)
include_directories(../safequeue)
include_directories(../safecout)
target_link_libraries(threadpool safequeue safecout)