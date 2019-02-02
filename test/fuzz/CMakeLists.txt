add_custom_target(f5-cord-fuzz)

add_executable(f5-cord-fuzz-u8loader u8loader.cpp)
target_link_libraries(f5-cord-fuzz-u8loader f5-cord)
add_dependencies(f5-cord-fuzz f5-cord-fuzz-u8loader)
