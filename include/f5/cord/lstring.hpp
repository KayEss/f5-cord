/**
    Copyright 2016-2018, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/cord/unicode-core.hpp>
#include <string>


namespace f5 {


    namespace cord {
        class lstring;
    }
    inline namespace literals {
        constexpr cord::lstring operator"" _l(const char *, std::size_t);
    }


    namespace cord {


        /// A wrapper around a string literal used for compile time string
        /// handling.
        class lstring {
          private:
            const char *p;
            std::size_t bytes;

            /// Used only by the literal
            friend constexpr lstring f5::literals::
                    operator"" _l(const char *, std::size_t);
            constexpr lstring(const char *s, std::size_t b) : p(s), bytes(b) {}

          public:
            /// Compile time construction
            constexpr lstring() : p(nullptr), bytes(0) {}
            template<std::size_t N>
            constexpr lstring(const char (&a)[N]) : p(a), bytes(N - 1) {}

            /// Data access
            constexpr std::size_t size() const { return bytes; }
            constexpr bool empty() const { return not bytes; }
            constexpr const char *data() const { return p; }
            constexpr const char *c_str() const { return p ? p : ""; }

            /// Convert to a std::string
            explicit operator std::string() const {
                return p ? std::string{c_str()} : std::string();
            }
            explicit operator std::string_view() const noexcept {
                return std::string_view(data(), size());
            }
            /// Safe conversions
            constexpr operator const_u8buffer() const noexcept {
                return const_u8buffer(p, bytes);
            }
            operator buffer<unsigned char const>() const noexcept {
                return {reinterpret_cast<unsigned char const *>(p), bytes};
            }

            constexpr bool operator==(lstring o) const {
                if (bytes == o.bytes) {
                    for (std::size_t s{}; s != bytes; ++s) {
                        if (p[s] != o.p[s]) return false;
                    }
                    return true;
                } else {
                    return false;
                }
            }
            template<std::size_t N>
            constexpr bool operator==(const char (&a)[N]) const {
                return *this == lstring{a};
            }
            bool operator==(const std::string &s) const { return s == c_str(); }
            constexpr bool operator!=(lstring o) const {
                return not(*this == o);
            }
            template<std::size_t N>
            constexpr bool operator!=(const char (&a)[N]) const {
                return *this != lstring{a};
            }
            bool operator!=(const std::string &s) const { return s != c_str(); }

            /// Ordering with an `lstring` on the left
            constexpr bool operator<(lstring o) const {
                using b = buffer<const char>;
                return b(p, bytes) < b(o.p, o.bytes);
            }
            bool operator<(const std::string &s) const { return c_str() < s; }
        };


        /// Comparisons with a `std::string` on the left
        inline bool operator==(const std::string &l, lstring r) {
            return l == r.c_str();
        }
        inline bool operator!=(const std::string &l, lstring r) {
            return l != r.c_str();
        }

        /// Ordering with a `std::string` on the left
        inline bool operator<(const std::string &l, lstring r) {
            return l < r.c_str();
        }
    }


    inline namespace literals {


        constexpr inline cord::lstring
                operator"" _l(const char *s, std::size_t n) {
            return cord::lstring(s, n);
        }


    }


    using lstring = cord::lstring;


}
