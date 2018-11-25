/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <iostream>


void assert_impl(bool a, const char *at, const char *f, unsigned l) {
    if (not a) {
        std::cout << f << ":" << l << " => assert(" << at << ") failed\n";
        std::exit(120);
    }
}

#ifdef assert // Apple, sigh...
#undef assert
#endif
#define assert(b) assert_impl(b, #b, __FILE__, __LINE__);
