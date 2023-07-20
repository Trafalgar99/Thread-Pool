cmake_minimum_required(VERSION 3.18)
add_subdirectory(threadpool)
add_subdirectory(safequeue)
add_subdirectory(safecout)

add_executable(main main.cpp)

include_directories(./safequeue)
include_directories(./threadpool)
include_directories(./safecout)

target_link_libraries(main threadpool safequeue safecout)