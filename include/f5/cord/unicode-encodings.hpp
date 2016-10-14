/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/cord/lstring.hpp>
#include <stdexcept>
#include <cstdint>


namespace f5 {


    /// Raise an error of type E giving it the specified error text
    template<typename E> [[noreturn]]
    constexpr inline
    void raise(f5::cord::lstring error) {
        throw E(error.c_str());
    }


    inline namespace cord {


        /// A UTF-8 code point
        typedef unsigned char utf8;
        /// A UTF-32 code point
        typedef uint32_t utf32;


        /// Check that the UTF32 code point is valid. Throw an exception if not.
        template<typename E = std::domain_error>
        constexpr inline
        bool check_valid(utf32 cp) {
            if ( cp >= 0xd800 && cp <= 0xdbff ) {
                raise<E>("UTF32 code point is in the leading UTF16 surrogate pair range");
            } else if ( cp >= 0xdc00 && cp <= 0xdfff ) {
                raise<E>("UTF32 code point is in the trailing UTF16 surrogate pair range");
            } else if ( cp == 0xfffe || cp == 0xffff ) {
                raise<E>("UTF32 code point is invalid");
            } else if ( cp > 0x10ffff ) {
                raise<E>("UTF32 code point is beyond the allowable range");
            } else {
                return true;
            }
        }

        /// Return the number of UTF8 values that this code point will
        /// occupy. If the code point falls in an invalid range then an
        /// exception will be thrown.
        template<typename E = std::domain_error>
        constexpr inline
        std::size_t u8length(utf32 cp) {
            check_valid<E>(cp);
            return 1u;
        }


    }


}

