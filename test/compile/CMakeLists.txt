add_library(cord-compile-tests STATIC EXCLUDE_FROM_ALL
        lstring-compare.cpp
        lstring.cpp
        memory.cpp
        tstring.cpp
        u8view.cpp
        unicode.cpp
    )
target_link_libraries(cord-compile-tests f5-cord)
add_dependencies(check cord-compile-tests)
