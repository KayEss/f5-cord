/**
    Copyright 2016-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "assert.hpp"

#include <f5/cord/lstring.hpp>

constexpr f5::lstring empty;
constexpr f5::lstring html = "html";
constexpr f5::lstring one = "one";


int main() {
    assert(std::string(empty).empty());

    assert(std::string(html) == html);
    assert(html == std::string(html));

    assert(std::string(html) != one);
    assert(html != std::string(one));

    assert(std::string(html) < one);
    assert(html < std::string(one));

    return 0;
}
