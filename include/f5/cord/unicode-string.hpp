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
        class u8string {
            u8shared buffer;
        public:
            /// ## Constructors
            u8string() {}

            explicit u8string(u8shared b) noexcept
            : buffer{b} {
            }

            explicit u8string(lstring l)
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
                friend class u8string;
                u8shared buffer;

                const_iterator(u8shared b)
                : buffer{b} {
                }

            public:
                const_iterator() {}
            };

            /// ## Queries
            bool empty() const {
                return buffer.size() == 0;
            }

            /// ## Substrings
            u8string substr(std::size_t, std::size_t) const {
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

