#include <cassert>
#include <f5/cord/unicode.hpp>


int main() {
    // u8encode
    assert((f5::u8encode(0) == std::array<f5::utf8, 4>{{0, 0, 0, 0}}));
    assert((f5::u8encode(' ') == std::array<f5::utf8, 4>{{0x20, 0, 0, 0}}));
    assert((f5::u8encode(0xa2) == std::array<f5::utf8, 4>{{0xc2, 0xa2, 0, 0}}));
    assert((f5::u8encode(0x20ac) == std::array<f5::utf8, 4>{{0xe2, 0x82, 0xac, 0}})); // euro
    assert((f5::u8encode(0x10348) == std::array<f5::utf8, 4>{{0xf0, 0x90, 0x8d, 0x88}}));

    return 0;
}

