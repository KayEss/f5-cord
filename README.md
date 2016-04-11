# f5-cord
Strings and their utilities (runtime and compile time)


## `f5::cord` ##


`cord` is an inline namespace in `f5` meaning it can be left out of the type names below.


### Compile time strings ###

These string classes offer different trade offs for creating and manipulating. Non-trivial compile time string manipulation requires a mixture of all of them for best efficiency. All of them operate on `char` types and all may hold zero values (`NIL`).

All of the string types may be compared using `constexpr` operator `==`.


#### `f5::cord::lstring` ####

A compile time string type for use with literals. These cannot be further manipulated, but they can be concatenated to yield a `vstring`.


#### `f5::cord::tstring` ####

A compile time string type where the characters can be manipulated. `tstrings` can be concatenated with other `tstring`s to make `tstring`s and they can also be converted to an `lstring`.

`tstring` instances can be concatenated to create new `tstring` instances. De-indexing is also `constexpr`.


#### `f5::cord::vstring` ####

A compile time string type made up of a concatenation of `lstring` values.

