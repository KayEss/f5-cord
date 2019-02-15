/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/unicode-view.hpp>

using namespace f5::literals;


constexpr f5::u8view empty;
static_assert(empty == "", "Empty");
static_assert(empty.empty(), "Empty");
static_assert(empty.bytes() == 0, "Empty");
static_assert(f5::u8view{""}.bytes() == 0, "Empty");

constexpr f5::u8view html = "html";
static_assert(html == "html", "Wrong result");
static_assert(not html.empty(), "Not empty");
static_assert(html.bytes() == 4, "Wrong size");
static_assert(f5::u8view{"html"}.bytes() == 4, "Wrong size");

static_assert(empty != html, "Wrong result");
