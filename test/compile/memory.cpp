#include <f5/memory.hpp>

constexpr const char *letters = "abcdefghijk";
constexpr f5::buffer<const char> abc(letters, 3);
constexpr auto bc = abc.slice(1);

static_assert(bc[0] == 'b', "Expected b");
static_assert(bc[1] == 'c', "Expected c");
static_assert(bc.size() == 2, "Expected 2 letters");
