# f5-cord

Strings and their utilities (runtime and compile time)

[![Build Status](https://travis-ci.org/KayEss/f5-cord.svg?branch=master)](https://travis-ci.org/KayEss/f5-cord)


## `f5::cord`

Most of the code and classes are defined in the `f5::cord` namespace, but some common use ones are also available under `f5`.


### Low level memory views

There are a few low level memory [span/view types and buffers](./include/f5/memory.hpp) used for dealing with contiguous runs of data.


### String handling



#### [`f5::cord::u8string`](./include/f5/cord/unicode-string.hpp)

A new vocabulary type for immutable Unicode strings using a shared ownership model. Because it has shared ownership, copying is much cheaper than for a normal string.

This type is also available as `f5::u8string`.


### [`f5::cord::u8view`](./include/f5/cord/unicode-view.hpp)

A new view class for Unicode strings. This class is mostly `constexpr` and is usable as a literals type.

It is also available as `f5::u8view`.


#### [`f5::cord::tstring`](./include/f5/cord/tstring.hpp)

A compile time string type where the characters can be manipulated. `tstrings` can be concatenated with other `tstring`s to make `tstring`s and they can also be converted to an `lstring`. They can be created from a string literal using the `_t` literal modifier:

    using namespace f5::literals;
    constexpr auto hello_world = "Hello, World!"_t;

`tstring` instances can be concatenated to create new `tstring` instances. De-indexing is also `constexpr`.


### Unicode utilities

For full details see [the Unicode documentation](include/f5/cord/unicode.md).

