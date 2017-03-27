/*
    Copyright 2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include "lstring.hpp"

#include <iostream>


template<class Ch, class Tr> inline
std::basic_ostream<Ch, Tr> &operator << (std::basic_ostream<Ch, Tr> &os, const f5::cord::lstring &ls) {
    return os << ls.c_str();
}

