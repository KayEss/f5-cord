/**
    Copyright 2019-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "assert.hpp"

#include <f5/cord/iostream.hpp>
#include <f5/cord/unicode-string.hpp>


int main() {
    f5::u32string e{};
    f5::u32string h{U"Hello"};
    assert(not h.is_shared());
    f5::u32string hw{std::u32string{U"Hello world"}};
    assert(hw.is_shared());
    const auto ce{e}, ch{h}, chw{hw};

    [hw](f5::u32view v) {
        assert(hw.is_shared());
        assert(v.is_shared());
        assert(v == hw);
        assert(v.data() == hw.data());

        f5::u32string nhw{v};
        assert(nhw.is_shared());
        assert(nhw.data() == hw.data());
        assert(nhw.shares_allocation_with(v));
        assert(nhw.shares_allocation_with(hw));
    }(hw);

    assert(e.empty());
    assert(ce.empty());

    assert(hw == U"Hello world");
    assert(U"Hello world" == hw);
    assert(hw != U"Hello");
    assert(chw == U"Hello world");
    assert(U"Hello world" == chw);
    assert(hw == chw);

    f5::u32string hwc{hw.begin(), hw.end()};
    assert(hw == hwc);
    assert(hw.shares_allocation_with(hwc));
    f5::u32view hwcc{hwc.begin(), hwc.end()};
    f5::u32string hwccc{hwcc.begin(), hwcc.end()};
    assert(hw == hwcc);
    assert(hw == hwccc);
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.is_shared());
    assert(hw.shares_allocation_with(hwccc));
    hwccc = f5::u32string{std::u32string(U"Next")};
    assert(hwccc == U"Next");
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.is_shared());
    assert(not hw.shares_allocation_with(hwccc));
    hwccc = hw;
    assert(hwccc == U"Hello world");
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.is_shared());
    assert(hw.shares_allocation_with(hwccc));

    f5::u32string gw{U"Goodbye world"};

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
        char32_t hel[] = {'H', 'e', 'l', 'l', 'o'};
        char32_t *p_hel = hel;
        for (auto c : h) { assert(c == *p_hel++); }
    }

    const auto clo = h.substr(3);
    assert(clo.bytes() == 8);
    assert(clo == U"lo");
    assert(hw.substr_pos(0, 3) == U"Hel");
    assert(hw.substr_pos(3, 5) == clo);
    assert(h.substr(10) == U"");
    assert(hw.substr(3) == hw.substr_pos(3, 123));

    auto lo = hw.substr_pos(3, 5);
    assert(lo.shares_allocation_with(hw));
    char32_t const *const plo = lo.shrink_to_fit();
    assert(plo[0] == 'l');
    assert(plo[1] == 'o');
    assert(plo[2] == 0);
    assert(not lo.shares_allocation_with(hw));
    assert(lo.shrink_to_fit() == plo);

    assert(hw.starts_with(U"Hello"));
    assert(not hw.ends_with(U"Hello world!"));
    assert(hw.ends_with(U"Hello world"));
    assert(not hw.ends_with(U"Hello World"));
    assert(hw.ends_with(U"world"));
    assert(not hw.ends_with(U"World"));

    assert(std::u32string(U"Hello") + hw.substr(5) == U"Hello world");
    {
        std::u32string s{U"Hello"};
        assert(s == chw.substr_pos(0, 5));
        assert(s != chw.substr(5));
        s += chw.substr(5);
        assert(s == U"Hello world");
    }

    f5::u32string h8{"Hello world \xF0\x9F\x98\x83"};
    f5::u32string h16{u"Hello world \xD83D\xDE03"};
    f5::u32string h32{U"Hello world \x1F603"};
    assert(h8 == h16);
    assert(h8 == h32);
    assert(h16 == h32);
}
