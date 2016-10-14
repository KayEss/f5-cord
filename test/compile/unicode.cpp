#include <f5/cord/unicode.hpp>

static_assert(f5::check_valid(' '), "Space is a valid UTF32 code point");
static_assert(not f5::check_valid<void>(0xff'ff'ff), "0xff'ff'ff is not a valid code point");

static_assert(f5::u8length(0) == 1, "Zero is still 1 byte long");
static_assert(f5::u8length(' ') == 1, "Space is 1 byte long");
