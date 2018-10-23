/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/unicode-string.hpp>

#include <cassert>


int main() {
    f5::u8string e{};
    f5::u8string h{f5::lstring{"Hello"}};
    f5::u8string hw{f5::lstring{"Hello world"}};
    const auto chw{hw}, ce{e};

    [](f5::u8view){}(hw);
    [](f5::u8shared){}(f5::u8shared{chw});

    assert(e.empty());
    assert(ce.empty());

    assert(hw == "Hello world");
    assert(hw != "Hello");
    assert(chw == "Hello world");
    assert(hw == chw);

    f5::u8string gw{f5::lstring{"Goodbye world"}};

    assert(gw < hw);
    assert(gw <= hw);
    assert(hw >= gw);
    assert(hw > gw);

    assert(gw < chw);
    assert(gw <= chw);
    assert(chw >= gw);
    assert(chw > gw);

    assert(hw <= hw);
    assert(hw >= hw);
    assert(h < hw);
    assert(hw > h);

//     assert(hw.substr(0, 3) == "Hel");
}

