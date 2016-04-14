#include <cassert>
#include <f5/cord/lstring.hpp>

constexpr f5::lstring empty;
constexpr f5::lstring html = "html";
constexpr f5::lstring html5 = "html5";


int main() {
    assert(html < html5);
    return 0;
}

