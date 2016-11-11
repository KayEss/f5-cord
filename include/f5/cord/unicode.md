# Unicode

    # include <f5/cord/unicode.hpp>

This [general purpose header](unicode.hpp) includes all of the Unicode functionality described below.


## Encodings

    # include <f5/cord/unicode-encodings.hpp>

[This header](unicode-encodings.hpp) includes support for handling encoding and decoding single code points.

C++ already includes types that are useful for UTF-16 and UTF-32, these being `char16_t` and `char32_t`. We alias these to `utf32` and `utf16`. For UTF-8 things aren't quite so happy though given that `char` may or may not signed. For consistency with `utf32` and `utf16`, `utf8` is defined as an `unsigned char`. This makes it distinct from the `char` type and not really compatible with `std::string`.

This causes a problem when we want to use `std::string` to represent a UTF-8 string. On platforms where `char` is a signed type it is difficult to process the data. For anything outside of the ASCII range we don't know what code unit values we're going to get. Despite this problem the standard is continuing to double down on the use of `char` for UTF-8.

