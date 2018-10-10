add_library(cord-headers-tests STATIC EXCLUDE_FROM_ALL
        iostream.cpp
        lstring.cpp
        tstring.cpp
        unicode-core.cpp
        unicode.cpp
        unicode-encodings.cpp
        unicode-iterators.cpp
        unicode-string.cpp
        unicode-view.cpp
    )
target_link_libraries(cord-headers-tests f5-cord)
add_dependencies(check cord-headers-tests)
