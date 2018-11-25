/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/cord/lstring.hpp>


namespace f5 {


    /// Raise an error of type E giving it the specified error text
    template<typename E>
    constexpr inline void raise(f5::cord::lstring error) {
        throw E(error.c_str());
    }

    /// Specialisation for when we want an error return
    template<>
    constexpr inline void raise<void>(f5::cord::lstring) {}


}
