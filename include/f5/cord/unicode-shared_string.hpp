/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/cord/unicode-view.hpp>

#include <algorithm>


namespace f5 {


    inline namespace cord {


        /// UTF8 string with shared ownership.
        class shared_string {
            u8shared buffer;
        public:
            /// ## Constructors
            explicit shared_string(u8shared b) noexcept
            : buffer{b} {
            }

            explicit shared_string(lstring l)
            : buffer{
                std::shared_ptr<const unsigned char>{
                    reinterpret_cast<const unsigned char *>(l.c_str()),
                    [](auto &&){}},
                l.size()}
            {}

            /// ## Substrings
            shared_string substr(std::size_t, std::size_t) const {
                return *this;
            }

            /// ## Comparisons
            bool operator == (u8view l) const {
                u8view m{*this};
                return std::equal(l.begin(), l.end(), m.begin(), m.end());
            }
            bool operator != (u8view l) const {
                return not ((*this) == l);
            }

            /// ## Conversions
            explicit operator u8shared () const noexcept {
                return buffer;
            }
            operator u8view () const {
                return u8view{const_u8buffer{buffer}};
            }
        };


    }


}

