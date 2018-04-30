add_compile_options(-fdiagnostics-show-option
    -Werror -Wall -Wextra -Wpedantic
    -Wno-gnu-string-literal-operator-template
    -Wno-unused-const-variable)

add_subdirectory(compile)
add_subdirectory(headers)
add_subdirectory(run)
