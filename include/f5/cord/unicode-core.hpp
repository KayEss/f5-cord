/*
    Copyright 2016-2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/memory.hpp>


namespace f5 {


    inline namespace cord {


        /// A UTF-8 code unit
        using utf8 = unsigned char;
        /// A UTF-16 code unit
        using utf16 = char16_t;
        /// A UTF-32 code unit (or code point)
        using utf32 = char32_t;

        /// Const memory buffers
        using u8buffer = array_view<utf8>;
        using u16buffer = array_view<utf16>;
        using u32buffer = array_view<utf32>;
        using const_u8buffer = array_view<const utf8>;
        using const_u16buffer = array_view<const utf16>;
        using const_u32buffer = array_view<const utf32>;


    }


}

