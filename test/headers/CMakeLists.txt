add_library(headers-tests STATIC EXCLUDE_FROM_ALL
        iostream.cpp
    )
target_link_libraries(headers-tests f5-cord)
add_dependencies(check headers-tests)
