cmake_minimum_required(VERSION 3.9)

add_library(f5-cord INTERFACE)
target_include_directories(f5-cord INTERFACE include)
target_compile_options(f5-cord INTERFACE -std=c++14)

add_subdirectory(test)
