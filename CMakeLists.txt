cmake_minimum_required(VERSION 3.9)

add_library(f5-cord INTERFACE)
target_include_directories(f5-cord INTERFACE include)

add_subdirectory(test)
