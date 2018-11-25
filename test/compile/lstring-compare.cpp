#include <f5/cord/lstring.hpp>

constexpr f5::lstring empty;
constexpr f5::lstring html = "html";
constexpr f5::lstring html4 = "html4";
constexpr f5::lstring html5 = "html5";
constexpr f5::lstring xhtml5 = "xhtml5";

static_assert(html == "html", "html == \"html\"");
static_assert(html4 != "html5", "html4 != html5");
static_assert(html4 != html5, "html4 != html5");

static_assert(empty < html, "empty < html");
static_assert(html < html4, "html < html4");
static_assert(html4 < html5, "html4 < html5");
static_assert(html5 < xhtml5, "html5 < xhtml5");

static_assert(not(html5 < empty), "not (html5 < empty)");
static_assert(not(html5 < html4), "not (html5 < html4)");
static_assert(not(xhtml5 < html5), "not (xhtml5 < html5)");
