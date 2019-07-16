/**
    Copyright 2016-2019, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "assert.hpp"

#include <algorithm>
#include <f5/memory.hpp>


template<typename T>
void tests(f5::shared_buffer<T> b1) {
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

    /// These are all safe operations
    assert(b1.slice(100).size() == 0u);
    assert(b1.slice(1, 100).size() == 9u);
    assert(b1.slice(100, 100).size() == 0u);

    f5::buffer<T> b4 = b1;
    assert(b4.size() == 10);
    f5::buffer<const T> b5 = b3;
    assert(b5.size() == 3);
    f5::buffer<T> b6 = b1.slice(6);
    assert(b6.size() == 4);

    f5::buffer<const T> b7 = b1;
    assert(b7.size() == 10);
}


int main() {
    int const items[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    f5::shared_buffer<int> buf{10};
    std::copy(std::begin(items), std::end(items), buf.begin());
    tests(buf);

    f5::shared_buffer<int const> cbuf{buf};
    tests(cbuf);

    return 0;
}
