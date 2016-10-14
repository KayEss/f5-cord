#include <f5/cord/unicode.hpp>


static_assert(f5::u8length(0) == 1, "Zero is still 1 byte long");
static_assert(f5::u8length(' ') == 1, "Space is 1 byte long");
