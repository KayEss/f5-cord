/**
    Copyright 2018-2019 Red Anchor Trading Co. Ltd.

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
static_assert(empty.code_units() == 0, "Empty");
static_assert(f5::u8view{""}.code_units() == 0, "Empty");
static_assert(f5::cord::size(empty) == 0, "Empty");

constexpr f5::u8view html = "html";
static_assert(html == "html", "Wrong result");
static_assert("html" == html, "Wrong result");
static_assert(not html.empty(), "Not empty");
static_assert(html.bytes() == 4, "Wrong size");
static_assert(f5::u8view{"html"}.bytes() == 4, "Wrong size");
static_assert(html.code_units() == 4, "Wrong size");
static_assert(f5::u8view{"html"}.code_units() == 4, "Wrong size");
static_assert(f5::cord::size(html) == 4, "Wrong size");

static_assert(empty != html, "Wrong result");
static_assert(f5::cord::data(empty) != f5::cord::data(html), "Wrong result");
static_assert("" != html, "Wrong result");

static_assert(empty.begin() == empty.end(), "Not empty");
static_assert(*html.begin() == 'h', "Wrong letter");
static_assert(*(++html.begin()) == 't', "Wrong letter");
static_assert(*(html.begin()++) == 'h', "Wrong letter");
