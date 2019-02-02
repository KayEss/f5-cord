#include <f5/cord/tstring.hpp>

constexpr f5::cord::tstring<> empty;
static_assert(empty == "", "Not empty");
static_assert(empty.size() == 0, "Not empty");

constexpr f5::cord::tstring<'h', 't', 'm', 'l'> html;
static_assert(html.as_lstring() == "html", "Wrong result");
static_assert(html == "html", "Wrong result");
static_assert(html.size() == 4, "Wrong size");

constexpr auto html5 = html + f5::cord::tstring<'5'>();
static_assert(html5.as_lstring() == "html5", "Wrong result");
static_assert(html5 == "html5", "Wrong result");
static_assert(html5.size() == 5, "Wrong size");

using namespace f5::literals;

constexpr auto hello = "Hello, "_t;
constexpr auto world = "World!"_t;
static_assert(hello != world, "Hello is World :(");
static_assert(not(hello == world), "Hello is World :(");
static_assert(hello + world == "Hello, World!"_t, "Not a welcoming world");
static_assert(not(hello + world != "Hello, World!"_t), "Not a welcoming world");
