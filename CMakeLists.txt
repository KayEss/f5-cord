cmake_minimum_required(VERSION 3.6)

add_library(f5-cord INTERFACE)
target_include_directories(f5-cord INTERFACE include)
target_compile_features(f5-cord INTERFACE cxx_std_14)

enable_testing()
add_custom_target(check)
set_property(TARGET check PROPERTY EXCLUDE_FROM_ALL TRUE)
add_subdirectory(test)
