/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/unicode-shared_string.hpp>

#include <cassert>


int main() {
    f5::shared_string hw{f5::lstring{"Hello world"}};
    const auto chw{hw};

    [](f5::u8view){}(hw);
    [](f5::u8shared){}(f5::u8shared{chw});

    assert(hw == "Hello world");
    assert(hw != "Hello");
    assert(chw == "Hello world");
    assert(hw == chw);

    f5::shared_string gw{f5::lstring{"Goodbye world"}};

    assert(gw < hw);
    assert(gw <= hw);
    assert(hw >= gw);
    assert(hw > gw);

    assert(gw < chw);
    assert(gw <= chw);
    assert(chw >= gw);
    assert(chw > gw);

//     assert(hw >= hw);
//     assert(hw <= hw);

//     assert(hw.substr(0, 3) == "Hel");
}

