add_compile_options(
        -fdiagnostics-show-option
        -Werror -Wall -Wextra -Wpedantic
        -Wno-gnu-string-literal-operator-template
        -Wno-unused-const-variable
        -Wno-unused-local-typedefs
    )
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    add_compile_options(
            -Wno-unused-but-set-variable
        )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options(
            -Wno-unused-variable
        )
endif()

add_subdirectory(compile)
add_subdirectory(fuzz)
add_subdirectory(headers)
add_subdirectory(run)
