#include <f5/cord/lstring.hpp>

constexpr f5::lstring html = "html";
static_assert(html == "html", "Wrong result");
static_assert(html.size() == 4, "Wrong size");

