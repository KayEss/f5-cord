/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/iostream.hpp>
#include <f5/cord/unicode-string.hpp>

#include "assert.hpp"


int main() {
    f5::u8string e{};
    f5::u8string h{f5::lstring{"Hello"}};
    assert(not h.is_shared());
    f5::u8string hw{std::string{"Hello world"}};
    assert(hw.is_shared());
    const auto ce{e}, ch{h}, chw{hw};

    [hw](f5::u8view v) {
        assert(hw.is_shared());
        assert(v.is_shared());
        assert(v == hw);
        assert(v.data() == hw.data());

        f5::u8string nhw{v};
        assert(nhw.is_shared());
        assert(nhw.data() == hw.data());
        assert(nhw.shares_allocation_with(v));
        assert(nhw.shares_allocation_with(hw));
    }(hw);

    assert(e.empty());
    assert(ce.empty());

    assert(hw == "Hello world");
    assert("Hello world" == hw);
    assert(hw != "Hello");
    assert(chw == "Hello world");
    assert("Hello world" == chw);
    assert(hw == chw);

    f5::u8string hwc{hw.begin(), hw.end()};
    assert(hw == hwc);
    assert(hw.shares_allocation_with(hwc));
    f5::u8view hwcc{hwc.begin(), hwc.end()};
    f5::u8string hwccc{hwcc.begin(), hwcc.end()};
    assert(hw == hwcc);
    assert(hw == hwccc);
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.shares_allocation_with(hwccc));

    f5::u8string gw{f5::lstring{"Goodbye world"}};

    assert(gw < hw);
    assert(gw <= hw);
    assert(hw >= gw);
    assert(hw > gw);

    assert(gw < chw);
    assert(gw <= chw);
    assert(chw >= gw);
    assert(chw > gw);

    assert(hw <= hw);
    assert(hw >= hw);
    assert(h < hw);
    assert(hw > h);

    {
        auto p = ch.begin();
        assert(*p == 'H');
        assert(*(++p) == 'e');
        assert(*(p++) == 'e');
        assert(*p == 'l');
    }

    {
        f5::utf32 hel[] = {'H', 'e', 'l', 'l', 'o'};
        f5::utf32 *p_hel = hel;
        for (auto c : h) { assert(c == *p_hel++); }
    }

    const auto chel = h.substr(3);
    assert(chel.bytes() == 2);
    assert(chel == "lo");
    assert(hw.substr(0, 3) == "Hel");
    assert(h.substr(10) == "");
    assert(hw.substr(3) == hw.substr(3, 123));

    assert(hw.starts_with("Hello"));

    assert(std::string("Hello") + hw.substr(5) == "Hello world");
    {
        std::string s{"Hello"};
        s += chw.substr(5);
        assert(s == "Hello world");
    }
}
