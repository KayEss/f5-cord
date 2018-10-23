/**
    Copyright 2016-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "assert.hpp"

#include <algorithm>
#include <f5/memory.hpp>


int main() {
    int items[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    f5::shared_buffer<int> b1{10};
    std::copy(std::begin(items), std::end(items), b1.begin());
    assert(b1[0] == 0);
    assert(b1[8] == 8);

    auto b2 = b1.slice(8);
    assert(b2.size() == 2);
    assert(b2[0] == 8);
    assert(b2[1] == 9);
    auto b3 = b1.slice(2, 3);
    assert(b3.size() == 3);
    assert(b3[0] == 2);
    assert(b3[1] == 3);
    assert(b3[2] == 4);

    f5::buffer<int> b4 = b1;
    assert(b4.size() == 10);
    f5::buffer<const int> b5 = b3;
    assert(b5.size() == 3);
    f5::buffer<int> b6 = b1.slice(6);
    assert(b6.size() == 4);

    f5::buffer<const int> b7 = b1;
    assert(b7.size() == 10);

    return 0;
}
