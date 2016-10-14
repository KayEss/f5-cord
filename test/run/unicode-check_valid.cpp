#include <cassert>
#include <f5/cord/unicode.hpp>


bool check_exception(f5::utf32 cp, const std::string &what) {
    try {
        f5::check_valid(cp);
        return false;
    } catch ( std::exception &e ) {
        return what == e.what();
    }
}


const std::string leading_pair = "UTF32 code point is in the leading UTF16 surrogate pair range";


int main() {
    // Normal characters
    assert(f5::check_valid(' '));

    // Surrogate pair characters
    assert(check_exception(0xda13, leading_pair));

    return 0;
}

