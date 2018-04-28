add_library(compile-tests STATIC
        lstring-compare.cpp
        lstring.cpp
        memory.cpp
        tstring.cpp
        unicode.cpp
    )
target_link_libraries(compile-tests f5-cord)
