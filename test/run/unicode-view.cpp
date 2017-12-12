/*
    Copyright 2016-2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <f5/cord/unicode-view.hpp>

#include <cassert>
#include <iostream>


int main() {
    f5::u8view t1{"123 \xe2\x9c\x93 321"};

    assert(t1 != (const char *)"123");
    assert(t1 != (const char *)"1234");
    assert(t1 != (const char *)"123 \xe2\x9c\x93 321 extra");
    assert(t1 == (const char *)"123 \xe2\x9c\x93 321");

    assert((std::string)t1 ==  "123 \xe2\x9c\x93 321");

    assert(t1.substr(0) == "123 \xe2\x9c\x93 321");
    assert(t1.substr(2) == "3 \xe2\x9c\x93 321");
    assert(t1.substr(6) == "321");
    assert(t1.substr(200) == "");

    assert(t1.substr(0, 200) == "123 \xe2\x9c\x93 321");
    assert(t1.substr(1, 3) == "23");
    assert(t1.substr(2, 7) == "3 \xe2\x9c\x93 3");

    return 0;
}

