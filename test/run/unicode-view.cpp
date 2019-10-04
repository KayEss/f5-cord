/**
    Copyright 2016-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */
/// # Unicode string tests
/**
 * The check mark character () is U2713. The UTF-8 sequence for it is 0xE2 0x9x
 * 0x93.
 */


#include "assert.hpp"

#include <f5/cord/unicode-string.hpp>


int main() {
    /// ## UTF-8
    f5::u8view t8{"123 \xe2\x9c\x93 321"};

    assert(t8 != "123");
    assert(t8 != "1234");
    assert(t8 != "123 \xe2\x9c\x93 321 extra");
    assert(t8 == "123 \xe2\x9c\x93 321");

    assert(t8.substr(0) == "123 \xe2\x9c\x93 321");
    assert(t8.substr(2) == "3 \xe2\x9c\x93 321");
    assert(t8.substr(6) == "321");
    assert(t8.substr(200) == "");

    assert(t8.substr_pos(0, 200) == "123 \xe2\x9c\x93 321");
    assert(t8.substr_pos(1, 3) == "23");
    assert(t8.substr_pos(2, 7) == "3 \xe2\x9c\x93 3");

    assert(std::string(t8) == "123 \xe2\x9c\x93 321");
    assert(std::string(t8) + t8.substr(6) == "123 \xe2\x9c\x93 321321");
    std::string s8{"ABC"};
    assert((s8 += t8) == "ABC123 \xe2\x9c\x93 321");

    /// ## UTF-16
    f5::cord::u16view t16{u"123 \x2713 321"};

    assert(t16 != u"123");
    assert(t16 != u"1234");
    assert(t16 != u"123 \x2713 321 extra");
    assert(t16 == u"123 \x2713 321");

    //     assert(t16.substr(0) == u"123 \x2713 321");
    //     assert(t16.substr(2) == u"3 \x2713 321");
    //     assert(t16.substr(6) == u"321");
    //     assert(t16.substr(200) == u"");

    return 0;
}
