/**
    Copyright 2016-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "assert.hpp"

#include <iostream>
#include <f5/cord/unicode.hpp>


int main() {
    using u8encoded = decltype(f5::cord::u8encode(0));
    assert((f5::cord::u8encode(0) == u8encoded{1, {{0, 0, 0, 0}}}));
    assert((f5::cord::u8encode(' ') == u8encoded{1, {{0x20, 0, 0, 0}}}));
    assert((f5::cord::u8encode(0xa2)
            == u8encoded{2,
                         {{static_cast<char>(0xc2), static_cast<char>(0xa2), 0,
                           0}}}));
    assert((f5::cord::u8encode(0x20ac)
            == u8encoded{3,
                         {{static_cast<char>(0xe2), static_cast<char>(0x82),
                           static_cast<char>(0xac), 0}}})); // euro
    assert((f5::cord::u8encode(0x10348)
            == u8encoded{4,
                         {{static_cast<char>(0xf0), static_cast<char>(0x90),
                           static_cast<char>(0x8d), static_cast<char>(0x88)}}}));
    assert((f5::cord::u8encode<void>(0xffffff) == u8encoded{0, {{0, 0, 0, 0}}}));

    using u16encoded = decltype(f5::cord::u16encode(0));
    assert((f5::cord::u16encode(0) == u16encoded{1, {{0, 0}}}));
    assert((f5::cord::u16encode(' ') == u16encoded{1, {{' ', 0}}}));
    assert((f5::cord::u16encode(0x10348) == u16encoded{2, {{0xD800, 0xDF48}}}));
    assert((f5::cord::u16encode(0x1D11E) == u16encoded{2, {{0xD834, 0xDD1E}}}));

    return 0;
}
