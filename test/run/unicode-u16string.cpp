/**
    Copyright 2019-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "assert.hpp"

#include <f5/cord/iostream.hpp>
#include <f5/cord/unicode-string.hpp>


int main() {
    f5::u16string e{};
    f5::u16string h{u"Hello"};
    assert(not h.is_shared());
    f5::u16string hw{std::u16string{u"Hello world"}};
    assert(hw.is_shared());
    const auto ce{e}, ch{h}, chw{hw};

    [hw](f5::u16view v) {
        assert(hw.is_shared());
        assert(v.is_shared());
        assert(v == hw);
        assert(v.data() == hw.data());

        f5::u16string nhw{v};
        assert(nhw.is_shared());
        assert(nhw.data() == hw.data());
        assert(nhw.shares_allocation_with(v));
        assert(nhw.shares_allocation_with(hw));
    }(hw);

    assert(e.empty());
    assert(ce.empty());

    assert(hw == u"Hello world");
    assert(u"Hello world" == hw);
    assert(hw != u"Hello");
    assert(chw == u"Hello world");
    assert(u"Hello world" == chw);
    assert(hw == chw);

    f5::u16string hwc{hw.begin(), hw.end()};
    assert(hw == hwc);
    assert(hw.shares_allocation_with(hwc));
    f5::u16view hwcc{hwc.begin(), hwc.end()};
    f5::u16string hwccc{hwcc.begin(), hwcc.end()};
    assert(hw == hwcc);
    assert(hw == hwccc);
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.is_shared());
    assert(hw.shares_allocation_with(hwccc));
    hwccc = f5::u16string{std::u16string(u"Next")};
    assert(hwccc == u"Next");
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.is_shared());
    assert(not hw.shares_allocation_with(hwccc));
    hwccc = hw;
    assert(hwccc == u"Hello world");
    assert(hw.shares_allocation_with(hwcc));
    assert(hw.is_shared());
    assert(hw.shares_allocation_with(hwccc));

    f5::u16string gw{u"Goodbye world"};

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
    assert(clo.bytes() == 4);
    assert(clo == u"lo");
    assert(hw.substr_pos(0, 3) == u"Hel");
    assert(hw.substr_pos(3, 5) == clo);
    assert(h.substr(10) == u"");
    assert(hw.substr(3) == hw.substr_pos(3, 123));

    auto lo = hw.substr_pos(3, 5);
    assert(lo.shares_allocation_with(hw));
    char16_t const *const plo = lo.shrink_to_fit();
    assert(plo[0] == 'l');
    assert(plo[1] == 'o');
    assert(plo[2] == 0);
    assert(not lo.shares_allocation_with(hw));
    assert(lo.shrink_to_fit() == plo);

    assert(hw.starts_with(u"Hello"));
    assert(not hw.ends_with(u"Hello world!"));
    assert(hw.ends_with(u"Hello world"));
    assert(not hw.ends_with(u"Hello World"));
    assert(hw.ends_with(u"world"));
    assert(not hw.ends_with(u"World"));

    assert(std::u16string(u"Hello") + hw.substr(5) == u"Hello world");
    {
        std::u16string s{u"Hello"};
        assert(s == chw.substr_pos(0, 5));
        assert(s != chw.substr(5));
        s += chw.substr(5);
        assert(s == u"Hello world");
    }

    f5::u16string h8{"Hello world \xF0\x9F\x98\x83"};
    f5::u16string h16{u"Hello world \xD83D\xDE03"};
    f5::u16string h32{U"Hello world \x1F603"};
    assert(h8 == h16);
    assert(h8 == h32);
    assert(h16 == h32);
}
