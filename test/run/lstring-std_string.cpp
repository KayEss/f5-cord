#include <cassert>
#include <f5/cord/lstring.hpp>

constexpr f5::lstring html = "html";


int main() {
    return std::string(html) == "html" ? 0 : 1;
}

