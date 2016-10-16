#include <cassert>
#include <f5/cord/unicode.hpp>


int main() {
    // u8encode
    using u8encoded = decltype(f5::u8encode(0));
    assert((f5::u8encode(0) == u8encoded{1, {{0, 0, 0, 0}}}));
    assert((f5::u8encode(' ') == u8encoded{1, {{0x20, 0, 0, 0}}}));
    assert((f5::u8encode(0xa2) == u8encoded{2, {{0xc2, 0xa2, 0, 0}}}));
    assert((f5::u8encode(0x20ac) == u8encoded{3, {{0xe2, 0x82, 0xac, 0}}})); // euro
    assert((f5::u8encode(0x10348) == u8encoded{4, {{0xf0, 0x90, 0x8d, 0x88}}}));
    assert((f5::u8encode<void>(0xffffff) == u8encoded{0, {{0, 0, 0, 0}}}));

    return 0;
}

