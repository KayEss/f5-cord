if(NOT CMAKE_VERSION VERSION_LESS "3.12")
    add_executable(f5-cord-wordlist wordlist.cpp)
    target_compile_features(f5-cord-wordlist PRIVATE cxx_std_20)
    target_link_libraries(f5-cord-wordlist f5-cord)
endif()
