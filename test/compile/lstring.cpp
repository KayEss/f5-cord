#include <f5/cord/lstring.hpp>

using namespace f5::literals;


constexpr f5::lstring empty;
static_assert(empty == "", "Empty");
static_assert(empty.empty(), "Empty");
static_assert(empty.size() == 0, "Empty");
static_assert((""_l).size() == 0, "Empty");

constexpr f5::lstring html = "html";
static_assert(html == "html", "Wrong result");
static_assert(not html.empty(), "Not empty");
static_assert(html.size() == 4, "Wrong size");
static_assert(("html"_l).size() == 4, "Wrong size");

static_assert(empty != html, "Wrong result");
