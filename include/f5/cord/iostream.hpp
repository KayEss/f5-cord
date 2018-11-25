/**
    Copyright 2017-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include "lstring.hpp"
#include "unicode-view.hpp"

#include <iosfwd>


namespace std {


    template<class Ch, class Tr>
    inline auto &
            operator<<(basic_ostream<Ch, Tr> &os, const f5::cord::lstring &ls) {
        return os << ls.c_str();
    }


    template<class Ch, class Tr>
    inline auto &operator<<(basic_ostream<Ch, Tr> &os, f5::cord::u8view s) {
        os.write(s.data(), s.bytes());
        return os;
    }


}
