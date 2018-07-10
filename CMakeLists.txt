cmake_minimum_required(VERSION 3.6)

add_library(f5-cord INTERFACE)
target_include_directories(f5-cord INTERFACE include)
if(CMAKE_VERSION VERSION_LESS "3.8")
    message(WARNING "This verison of cmake is too old to request C++17.")
    ## This module doesn't actually use generic lambdas, but cmake 3.6
    ## (which is what we need to use for Android) doesn't support a test
    ## for the C++14 features we actually need and it doesn't support
    ## the `cxx_std_14` feature test either, so this is the work around.
    target_compile_features(f5-cord INTERFACE
        cxx_constexpr cxx_generic_lambdas)
else()
    target_compile_features(f5-cord INTERFACE cxx_std_17)
endif()

enable_testing()
add_custom_target(check)
set_property(TARGET check PROPERTY EXCLUDE_FROM_ALL TRUE)
add_subdirectory(test)
