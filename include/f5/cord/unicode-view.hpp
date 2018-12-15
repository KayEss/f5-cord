/**
    Copyright 2017-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/control.hpp>
#include <f5/memory.hpp>
#include <f5/cord/lstring.hpp>
#include <f5/cord/unicode-encodings.hpp>
#include <f5/cord/unicode-iterators.hpp>

#include <algorithm>
#include <cstring>


namespace f5 {


    inline namespace cord {


        class u8string;


        /// For unsigned char types with an UTF-8 encoding
        class u8view {
            friend class u8string;

            const_u8buffer buffer;
            using control_type = control<unsigned char const>;
            control_type *owner = nullptr;

            u8view(const_u8buffer b, control_type *o) : buffer{b}, owner{o} {}

          public:
            u8view() : buffer{}, owner{} {}

            explicit u8view(const_u8buffer b) : buffer(b), owner{} {}

            template<std::size_t N>
            u8view(const char (&s)[N])
            : buffer(reinterpret_cast<unsigned char const *>(s), N - 1),
              owner{} {}

            explicit u8view(const char *b, std::size_t s)
            : buffer(reinterpret_cast<unsigned char const *>(b), s), owner{} {}

            explicit u8view(const std::string &u8)
            : buffer(reinterpret_cast<unsigned char const *>(u8.data()),
                     u8.size()),
              owner{} {}

            u8view(lstring s)
            : buffer(reinterpret_cast<unsigned char const *>(s.c_str()),
                     s.size()),
              owner{} {}


            /// ## Iterators

            /// An iterator that spits out UTF32 code points from the string
            using const_iterator = const_u32_iterator<const_u8buffer>;

            /// Return the begin iterator that delivers UTF32 code points
            const_iterator begin() const { return const_iterator{buffer}; }
            /// Return the end iterator that delivers UTF32 code points
            const_iterator end() const {
                return const_iterator{buffer.slice(buffer.size())};
            }

            /// Construct a u8view from part of another
            u8view(const_iterator s, const_iterator e)
            : buffer(s.buffer.data(), s.buffer.size() - e.buffer.size()) {}

            /// An iterator that produces UTF16 code points from the string
            using const_u16_iterator =
                    f5::const_u32u16_iterator<const_iterator>;

            /// Return the begin iterator that delivers UTF16 code points
            const_u16_iterator u16begin() const {
                return const_u16_iterator(begin(), end());
            }
            /// Return the end iterator that delivers UTF16 code points
            const_u16_iterator u16end() const {
                return const_u16_iterator(end(), end());
            }

            /// ## Queries

            /// Returns `true` if this view is over a `u8string` and is a shared
            /// string
            bool is_shared() const { return owner != nullptr; }

            /// Return the data array
            const char *data() const noexcept {
                return reinterpret_cast<const char *>(buffer.data());
            }
            /// Return the size in bytes of the string
            std::size_t bytes() const noexcept { return buffer.size(); }
            /// Return the size in code points
            auto code_points() const { return std::distance(begin(), end()); }
            /// Return true if the view is empty
            bool empty() const noexcept { return buffer.empty(); }
            /// Return the underlying memory block for the data
            auto memory() const { return buffer; }

            /// ## Comparisons
            /// Comparison. Acts as a string would. Not unicode aware in
            /// that it doesn't take into account normalisation, it only
            /// compares the byte values.
            bool operator==(u8view r) const {
                return std::equal(
                        buffer.begin(), buffer.end(), r.buffer.begin(),
                        r.buffer.end());
            }
            bool operator!=(u8view r) const { return not((*this) == r); }
            bool operator==(const char *s) const {
                std::size_t pos{};
                for (; pos < buffer.size() && *s; ++pos, ++s) {
                    if (buffer[pos] != (unsigned char)*s) return false;
                }
                return pos == buffer.size() && *s == 0;
            }
            bool operator!=(const char *s) const { return not((*this) == s); }

            bool operator<(f5::u8view r) const { return buffer < r.buffer; }
            bool operator<=(f5::u8view r) const { return buffer <= r.buffer; }
            bool operator>=(f5::u8view r) const { return buffer >= r.buffer; }
            bool operator>(f5::u8view r) const { return buffer > r.buffer; }

            /// Useful checks for parts of a string
            bool starts_with(u8view str) const {
                return u8view{buffer.slice(0, str.buffer.size())} == str;
            }

            /// Safe substring against Unicode code point counts. The result
            /// undefined if the end marker is smaller than the start marker.
            u8view substr(std::size_t s) {
                auto pos = begin(), e = end();
                for (; s && pos != e; --s, ++pos)
                    ;
                return u8view(pos, e);
            }
            u8view substr(std::size_t s, std::size_t e) {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return u8view(starts.data(), ends.data() - starts.data());
            }

            /// Safe conversions
            operator const_u8buffer() const { return buffer; }

            /// Other conversions
            explicit operator std::string_view() const noexcept {
                return std::string_view(
                        reinterpret_cast<const char *>(buffer.data()),
                        buffer.size());
            }
            explicit operator std::string() const {
                return std::string(
                        buffer.data(), buffer.data() + buffer.size());
            }
        };


        /// Equality against other types
        inline bool operator==(lstring l, u8view r) { return r.operator==(l); }
        inline bool operator!=(lstring l, u8view r) { return r.operator!=(l); }

        /// Comparison against other types
        inline bool operator<(lstring l, u8view r) { return u8view(l) < r; }

        /// Concatenations with std::string
        inline std::string operator+(std::string f, u8view e) {
            f.append(e.data(), e.bytes());
            return f;
        }
        inline std::string operator+(u8view f, u8view e) {
            std::string r;
            r.reserve(f.bytes() + e.bytes());
            r.append(f.data(), f.bytes());
            r.append(e.data(), e.bytes());
            return r;
        }

        inline std::string &operator+=(std::string &s, u8view e) {
            s.append(e.data(), e.bytes());
            return s;
        }


    }


}
