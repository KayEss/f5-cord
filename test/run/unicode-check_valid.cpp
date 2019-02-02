/**
    Copyright 2016-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "assert.hpp"

#include <f5/cord/unicode.hpp>


bool check_exception(char32_t cp, const std::string &what) {
    try {
        f5::cord::check_valid(cp);
        return false;
    } catch (std::exception &e) { return what == e.what(); }
}


const std::string leading_pair =
        "UTF32 code point is in the leading UTF16 surrogate pair range";
const std::string trailing_pair =
        "UTF32 code point is in the trailing UTF16 surrogate pair range";
const std::string too_big = "UTF32 code point is beyond the allowable range";


int main() {
    // Normal characters
    assert(f5::cord::check_valid(' '));

    // Surrogate pair characters
    assert(check_exception(0xda13, leading_pair));
    assert(check_exception(0xdd13, trailing_pair));

    // Too big
    assert(check_exception(0xffffff, too_big));

    // Try invalid with void error type
    assert(f5::cord::check_valid<void>(' '));
    assert(not f5::cord::check_valid<void>(0xffffff));

    return 0;
}
