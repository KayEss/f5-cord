# Memory

## `buffer`

A view into a contiguous block of memory containing some type `V`. The underlying type may be marked `const` to show that the buffer content is immutable.

    #include <f5/memory.hpp>

    constexpr const char * letters = "abcdefghijk";
    constexpr f5::buffer<const char> abc(letters, 3);
    constexpr auto bc = abc.slice(1);


## `shared_buffer`

Shared ownership of a memory block that can be split into parts using `buffer`. It supports `slice`s which return shared buffers so the buffer can be split arbitrarily.

There are also automatic conversions to `buffer` types.

    #include <f5/memory.hpp>

    int items[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    f5::shared_buffer<int> b1{10};
    std::copy(std::begin(items), std::end(items), b1.begin());

    auto b2 = b1.slice(8);

The type may not be a const type.
