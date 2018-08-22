add_library(cord-headers-tests STATIC EXCLUDE_FROM_ALL
        iostream.cpp
    )
target_link_libraries(cord-headers-tests f5-cord)
add_dependencies(check cord-headers-tests)
