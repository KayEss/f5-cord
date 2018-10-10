/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/cord/unicode-view.hpp>


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

            /// ## Iterator
            class const_iterator : public std::iterator<
                    std::forward_iterator_tag,
                    utf32,
                    std::ptrdiff_t,
                    const utf32 *,
                    utf32>
            {
                friend class shared_string;
                u8shared buffer;

                const_iterator(u8shared b)
                : buffer{b} {
                }

            public:
                const_iterator() {}
            };

            /// ## Substrings
            shared_string substr(std::size_t, std::size_t) const {
                return *this;
            }

            /// ## Comparisons
            bool operator == (u8view l) const {
                return u8view{const_u8buffer{buffer}} == l;
            }
            bool operator != (u8view l) const {
                return u8view{const_u8buffer{buffer}} != l;
            }
            bool operator < (u8view l) const {
                return u8view{const_u8buffer{buffer}} < l;
            }
            bool operator <= (u8view l) const {
                return u8view{const_u8buffer{buffer}} <= l;
            }
            bool operator >= (u8view l) const {
                return u8view{const_u8buffer{buffer}} >= l;
            }
            bool operator > (u8view l) const {
                return u8view{const_u8buffer{buffer}} > l;
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

