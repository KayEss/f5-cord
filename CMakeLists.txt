cmake_minimum_required(VERSION 3.9)

add_library(f5-cord INTERFACE)
target_include_directories(f5-cord INTERFACE include)

enable_testing()
add_subdirectory(test/compile)
add_subdirectory(test/headers)
add_subdirectory(test/run)
