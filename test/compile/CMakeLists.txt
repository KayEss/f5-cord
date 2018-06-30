add_library(compile-tests STATIC EXCLUDE_FROM_ALL
        lstring-compare.cpp
        lstring.cpp
        memory.cpp
        tstring.cpp
        unicode.cpp
    )
target_link_libraries(compile-tests f5-cord)
add_dependencies(check compile-tests)
