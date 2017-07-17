/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/cord/unicode-core.hpp>
#include <string>


namespace f5 {


    inline namespace cord {


        /// A wrapper around a string literal used for compile time string
        /// handling.
        class lstring {
        private:
            const char *p;
            std::size_t bytes;

        public:
            constexpr lstring()
            : p(nullptr), bytes(0) {
            }
            template<std::size_t N>
            constexpr lstring(const char(&a)[N])
            : p(a), bytes(N-1) {
            }
            constexpr std::size_t size() const {
                return bytes;
            }

            constexpr const char * c_str() const {
                return p ? p : "";
            }

            /// Convert to a std::string (should this be explicit?)
            operator std::string () const {
                return p ? std::string{c_str()} : std::string();
            }
            /// Save conversions
            operator const_u8buffer () const {
                return const_u8buffer(reinterpret_cast<const unsigned char *>(p), bytes);
            }

            constexpr bool operator == (lstring o) const {
                if ( bytes == o.bytes ) {
                    for ( std::size_t s{}; s != bytes; ++s ) {
                        if ( p[s] != o.p[s] ) return false;
                    }
                    return true;
                } else {
                    return false;
                }
            }
            constexpr bool operator != (lstring o) const {
                return not (*this == o);
            }

            constexpr bool operator < (lstring o) const {
                const auto checks = bytes < o.bytes ? bytes : o.bytes;
                for ( std::size_t s{}; s != checks; ++s ) {
                    if ( p[s] != o.p[s] ) return p[s] < o.p[s];
                }
                return bytes < o.bytes;
            }
            bool operator < (const std::string &s) const {
                return p < s;
            }
        };


        inline bool operator < (const std::string &l, lstring r) {
            return l < r.c_str();
        }
    }


}

