# Unicode

    # include <f5/cord/unicode.hpp>

This [general purpose header](unicode.hpp) includes all of the Unicode functionality described below.


## Base types

Unicode code points are presented in an encoding which is made up of code units. The types for the code units are:

    using utf8 = unsigned char;
    using utf16 = char16_t;
    using utf32 = char32_t;

This makes all of these unisgned types, and because `unsigned char` and `char` are distinct types (even on platforms where `char` is unsigned) this causes some problems with interoperability with `std` string types and string literals, but makes it far less likely that there will be bugs in code that deals with the text.

The following are also provided as types for buffers containing code units:

    using u8buffer = buffer<utf8>;
    using u16buffer = buffer<utf16>;
    using u32buffer = buffer<utf32>;
    using const_u8buffer = buffer<const utf8>;
    using const_u16buffer = buffer<const utf16>;
    using const_u32buffer = buffer<const utf32>;

The first three are mutable buffers and the latter immutable. Added to these are shared immutable strings:

    using u8shared = shared_buffer<utf8>;
    using u16shared = shared_buffer<utf16>;
    using u32shared = shared_buffer<utf32>;

These share the data between all string instances, even when substringed. They do not however have a real string based API--their API is based only on that for the `shared_buffer`.

The string views found in `unicode-view.hpp` (see below) will take any of these buffer types and provide a more string like API.


## Encodings

    # include <f5/cord/unicode-encodings.hpp>

[This header](unicode-encodings.hpp) includes support for handling encoding and decoding single code points.

C++ already includes types that are useful for UTF-16 and UTF-32, these being `char16_t` and `char32_t`. We alias these to `utf32` and `utf16`. For UTF-8 things aren't quite so happy though given that `char` may or may not signed. For consistency with `utf32` and `utf16`, `utf8` is defined as an `unsigned char`. This makes it distinct from the `char` type and not really compatible with `std::string`.

This causes a problem when we want to use `std::string` to represent a UTF-8 string. On platforms where `char` is a signed type it is difficult to process the data. For anything outside of the ASCII range we don't know what code unit values we're going to get. Despite this problem the standard is continuing to double down on the use of `char` for UTF-8.


# Views

Currently there is only a `u8view`. The default iteration produces UTF32 code points, but there is provision to produce UTF16 code units as well through the `const_u16_iterator` iterators.

There is no `size` member as it would be confusing as to what is returned (code units or points), but there is a `bytes` member that returns the number of bytes.



