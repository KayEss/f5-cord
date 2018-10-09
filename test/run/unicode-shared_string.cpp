/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/unicode-shared_string.hpp>

#include <cassert>


void nop_u8view(f5::u8view) {
}


int main() {
    f5::shared_string hw{f5::lstring{"Hello world"}};
    nop_u8view(hw);
}

