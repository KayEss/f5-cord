add_library(headers-tests STATIC
        iostream.cpp
    )
target_link_libraries(headers-tests f5-cord)
