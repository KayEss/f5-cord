#include <cassert>
#include <f5/cord/lstring.hpp>
#include <f5/cord/unicode-view.hpp>


constexpr f5::lstring empty;
constexpr f5::lstring html = "html";
constexpr f5::lstring html5 = "html5";

constexpr f5::lstring now = "op:now";
constexpr f5::lstring set = "op:set";


int main() {
    assert(html < std::string(html5));
    assert(std::string(html) < html5);

    assert(now < std::string("op:set"));
    assert(not (now < std::string("op:now")));
    assert(not (set < std::string("op:set")));
    assert(not (set < std::string("op:now")));

    assert(html5 == f5::u8view(html5));
    assert(f5::u8view(html5) == html5);

    assert(html < f5::u8view(html5));
    assert(f5::u8view(html) < html5);

    return 0;
}

