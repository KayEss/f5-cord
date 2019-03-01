/**
    Copyright 2016-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/unicode-string.hpp>

#include "assert.hpp"


int main() {
    f5::u8view t1{"123 \xe2\x9c\x93 321"};

    assert(t1 != "123");
    assert(t1 != "1234");
    assert(t1 != "123 \xe2\x9c\x93 321 extra");
    assert(t1 == "123 \xe2\x9c\x93 321");

    assert(t1.substr(0) == "123 \xe2\x9c\x93 321");
    assert(t1.substr(2) == "3 \xe2\x9c\x93 321");
    assert(t1.substr(6) == "321");
    assert(t1.substr(200) == "");

    assert(t1.substr_pos(0, 200) == "123 \xe2\x9c\x93 321");
    assert(t1.substr_pos(1, 3) == "23");
    assert(t1.substr_pos(2, 7) == "3 \xe2\x9c\x93 3");

    assert(std::string(t1) == "123 \xe2\x9c\x93 321");
    assert(std::string(t1) + t1.substr(6) == "123 \xe2\x9c\x93 321321");
    std::string s1{"ABC"};
    assert((s1 += t1) == "ABC123 \xe2\x9c\x93 321");

    return 0;
}
