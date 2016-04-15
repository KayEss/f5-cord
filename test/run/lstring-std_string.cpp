#include <cassert>
#include <f5/cord/lstring.hpp>

constexpr f5::lstring empty;
constexpr f5::lstring html = "html";


int main() {
    assert(std::string(empty).empty());

    assert(std::string(html) == "html");

    std::string shtml = html;
    assert(std::string(shtml) == "html");

    return 0;
}

