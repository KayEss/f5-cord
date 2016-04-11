#include <f5/cord/tstring.hpp>

const f5::tstring<> empty;
static_assert(empty == "", "Not empty");
static_assert(empty.size() == 0, "Not empty");

const f5::tstring<'h', 't', 'm', 'l'> html;
static_assert(html.as_lstring() == "html", "Wrong result");
static_assert(html == "html", "Wrong result");
static_assert(html.size() == 4, "Wrong size");

const auto html5 = html + f5::tstring<'5'>();
static_assert(html5.as_lstring() == "html5", "Wrong result");
static_assert(html5 == "html5", "Wrong result");
static_assert(html5.size() == 5, "Wrong size");
