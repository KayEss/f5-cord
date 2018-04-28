add_executable(lstring-compare lstring-compare.cpp)
target_link_libraries(lstring-compare f5-cord)
add_test(lstring-compare-test lstring-compare)

add_executable(lstring-std_string lstring-std_string.cpp)
target_link_libraries(lstring-std_string f5-cord)
add_test(lstring-std_string-test lstring-std_string)

add_executable(memory memory.cpp)
target_link_libraries(memory f5-cord)
add_test(memory-test memory)

add_executable(unicode-check_valid unicode-check_valid.cpp)
target_link_libraries(unicode-check_valid f5-cord)
add_test(unicode-check_valid-test unicode-check_valid)

add_executable(unicode-encoding unicode-encoding.cpp)
target_link_libraries(unicode-encoding f5-cord)
add_test(unicode-encoding-test unicode-encoding)

add_executable(unicode-view unicode-view.cpp)
target_link_libraries(unicode-view f5-cord)
add_test(unicode-view-test unicode-view)
