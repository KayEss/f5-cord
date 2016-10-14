/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <stdexcept>


namespace f5 {


    inline namespace cord {


        /// A UTF-8 code point
        typedef unsigned char utf8;
        /// A UTF-32 code point
        typedef uint32_t utf32;


        /// Check that the UTF32 code point is valid. Throw an exception if not
        template<typename E = std::domain_error>
        constexpr inline bool check_valid(utf32 cp) {
            if ( cp >= 0xd800 && cp <= 0xdbff ) {
                throw E("UTF32 code point is in the leading UTF16 surrogate pair range");
            } else {
                return true;
            }
        }

        template<typename E = std::domain_error>
        constexpr inline std::size_t u8length(utf32 cp) {
            check_valid<E>(cp);
            return 1u;
        }


    }


}

