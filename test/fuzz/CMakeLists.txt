add_custom_target(f5-cord-fuzz)
set_property(TARGET f5-cord-fuzz PROPERTY EXCLUDE_FROM_ALL TRUE)

add_executable(f5-cord-fuzz-u8loader EXCLUDE_FROM_ALL u8loader.cpp)
target_link_libraries(f5-cord-fuzz-u8loader f5-cord)
add_dependencies(f5-cord-fuzz f5-cord-fuzz-u8loader)

if(TARGET check)
    add_dependencies(check f5-cord-fuzz)
endif()
