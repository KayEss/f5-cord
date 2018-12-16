/**
    Copyright 2016-2018, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/memory.hpp>


namespace f5 {


    namespace cord {


        /// A UTF-8 code unit
        using utf8 = char;
        /// A UTF-16 code unit
        using utf16 = char16_t;
        /// A UTF-32 code unit (or code point)
        using utf32 = char32_t;

        /// Const memory buffers
        using u8buffer = buffer<utf8>;
        using u16buffer = buffer<utf16>;
        using u32buffer = buffer<utf32>;
        using const_u8buffer = buffer<const utf8>;
        using const_u16buffer = buffer<const utf16>;
        using const_u32buffer = buffer<const utf32>;

        /// Shared immutable strings
        using u8shared = shared_buffer<const utf8>;
        using u16shared = shared_buffer<const utf16>;
        using u32shared = shared_buffer<const utf32>;


    }


    using utf32 = cord::utf32;
    using u8shared = cord::u8shared;


}
