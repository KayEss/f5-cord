# Memory

## `buffer`

A view into a contiguous block of memory containing some type `V`. The underlying type may be marked `const` to show that the buffer content is immutable.

    #include <f5/memory.hpp>

    constexpr const char * letters = "abcdefghijk";
    constexpr f5::buffer<const char> abc(letters, 3);
    constexpr auto bc = abc.slice(1);

