/*
    Copyright 2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include "lstring.hpp"
#include "unicode-view.hpp"

#include <iostream>


namespace std {


    template<class Ch, class Tr> inline
    auto &operator << (basic_ostream<Ch, Tr> &os, const f5::cord::lstring &ls) {
        return os << ls.c_str();
    }


    template<class Ch, class Tr> inline
    auto &operator << (basic_ostream<Ch, Tr> &os, f5::cord::u8view s) {
        for ( auto c : s )
            os << c;
        return os;
    }


}
