# Memory

## `array_view`

A view into a contiguous block of memory containing some type `V`.

    #include <f5/memory.hpp>

    constexpr const char * letters = "abcdefghijk";
    constexpr f5::array_view<const char> abc(letters, 3);
    constexpr auto bc = abc.slice(1);


