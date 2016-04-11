#include <f5/cord/lstring.hpp>

constexpr f5::lstring empty;
static_assert(empty == "", "Not empty");
static_assert(empty.size() == 0, "Not empty");

constexpr f5::lstring html = "html";
static_assert(html == "html", "Wrong result");
static_assert(html.size() == 4, "Wrong size");

