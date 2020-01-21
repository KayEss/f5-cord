/**
    Copyright 2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "assert.hpp"

#include <f5/cord/unicode-string.hpp>

#include <optional>


int main() {
    std::optional<f5::u8string> empty, x{"x"};
    assert(empty != "");
    assert(x == "x");

    return 0;
}
