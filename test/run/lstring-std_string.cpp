#include <cassert>
#include <f5/cord/lstring.hpp>

constexpr f5::lstring empty;
constexpr f5::lstring html = "html";
constexpr f5::lstring one = "one";


int main() {
    assert(std::string(empty).empty());

    assert(std::string(html) == html);
    assert(html == std::string(html));

    assert(std::string(html) != one);
    assert(html != std::string(one));

    assert(std::string(html) < one);
    assert(html < std::string(one));

    return 0;
}

