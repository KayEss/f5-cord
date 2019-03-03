/**
    Copyright 2017-2019, Felspar Co Ltd. <https://kirit.com/f5>

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

#ifndef assert
#include <cassert>
#endif


namespace f5 {


    namespace cord {


        class u8string;


        /// For unsigned char types with an UTF-8 encoding
        class u8view {
            friend class u8string;

            const_u8buffer buffer;
            using control_type = control<std::size_t>;
            control_type *owner = nullptr;

            u8view(const_u8buffer b, control_type *o) : buffer{b}, owner{o} {}

          public:
            /// ## Constructors

            constexpr u8view() noexcept : buffer{}, owner{} {}

            constexpr explicit u8view(const_u8buffer b) noexcept
            : buffer(b), owner{} {}

            template<std::size_t N>
            constexpr u8view(const char (&s)[N]) noexcept
            : buffer(s, N - 1), owner{} {}

            constexpr explicit u8view(const char *b, std::size_t s) noexcept
            : buffer(b, s), owner{} {}

            constexpr u8view(lstring s) noexcept
            : buffer(s.data(), s.size()), owner{} {}


            /// ## Conversions
            explicit operator const_u8buffer() const { return buffer; }
            explicit operator f5::buffer<byte const>() const {
                return {reinterpret_cast<byte const *>(buffer.data()),
                        buffer.size()};
            }
            explicit operator std::string_view() const noexcept {
                return std::string_view(buffer.data(), buffer.size());
            }
            explicit operator std::string() const {
                return std::string(
                        buffer.data(), buffer.data() + buffer.size());
            }


            /// ## Iteration

            /// An iterator that spits out UTF32 code points from the string
            using const_iterator =
                    const_u32_iterator<const_u8buffer, control_type>;

            /// Return the begin iterator that delivers UTF32 code points
            const_iterator begin() const {
                return const_iterator{buffer, owner};
            }
            /// Return the end iterator that delivers UTF32 code points
            const_iterator end() const {
                return const_iterator{buffer.slice(buffer.size()), owner};
            }

            /// Construct a u8view from part of another
            constexpr u8view(const_iterator s, const_iterator e) noexcept
            : buffer(s.buffer.data(), s.buffer.size() - e.buffer.size()),
              owner(s.owner) {
                assert(s.owner == e.owner);
            }

            /// An iterator that produces UTF16 code points from the string
            using const_u16_iterator = const_u32u16_iterator<const_iterator>;

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
            bool is_shared() const noexcept { return owner != nullptr; }
            /// Returns true if the other string uses the same allocation
            /// as this (they have the same control block).
            bool shares_allocation_with(u8view v) noexcept {
                return owner != nullptr && owner == v.owner;
            }

            /// Return the data array
            constexpr const char *data() const noexcept {
                return buffer.data();
            }
            /// Return the size in bytes of the string
            constexpr std::size_t bytes() const noexcept {
                return buffer.size();
            }
            /// Return the size in code points
            auto code_points() const { return std::distance(begin(), end()); }
            /// Return true if the view is empty
            constexpr bool empty() const noexcept { return buffer.empty(); }
            /// Return the underlying memory block for the data
            auto memory() const noexcept {
                return f5::buffer<byte const>{
                        reinterpret_cast<byte const *>(buffer.data()),
                        buffer.size()};
            }


            /// ## Substrings

            /// Safe substring against Unicode code point counts. The result
            /// is undefined if the end marker is smaller than the start marker.
            u8view substr(std::size_t s) const {
                auto pos = begin(), e = end();
                for (; s && pos != e; --s, ++pos)
                    ;
                return u8view(pos, e);
            }
            u8view substr_pos(std::size_t s, std::size_t e) const {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return u8view{starts.begin(), ends.begin()};
            }
            [[deprecated("Use substr_pos")]] auto
                    substr(std::size_t s, std::size_t e) const {
                return substr_pos(s, e);
            }


            /// ## Comparisons

            /// Comparison. Acts as a string would. Not unicode aware in
            /// that it doesn't take into account normalisation, it only
            /// compares the byte values.
            constexpr bool operator==(u8view r) const noexcept {
                if (buffer.size() == r.buffer.size()) {
                    if (buffer.data() != r.buffer.data()) {
                        for (std::size_t s{}; s != buffer.size(); ++s) {
                            if (buffer[s] != r.buffer[s]) return false;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
            }
            constexpr bool operator!=(u8view r) const noexcept {
                return not((*this) == r);
            }
            bool operator==(const std::string &r) const noexcept {
                return *this == u8view{r.data(), r.size()};
            }
            bool operator!=(const std::string &r) const noexcept {
                return *this != u8view{r.data(), r.size()};
            }
            constexpr bool operator==(const char *s) const noexcept {
                std::size_t pos{};
                for (; pos < buffer.size() && *s; ++pos, ++s) {
                    if (buffer[pos] != *s) return false;
                }
                return pos == buffer.size() && *s == 0;
            }
            constexpr bool operator!=(const char *s) const noexcept {
                return not((*this) == s);
            }

            constexpr bool operator<(u8view r) const {
                return buffer < r.buffer;
            }
            constexpr bool operator<=(u8view r) const {
                return buffer <= r.buffer;
            }
            constexpr bool operator>=(u8view r) const {
                return buffer >= r.buffer;
            }
            constexpr bool operator>(u8view r) const {
                return buffer > r.buffer;
            }

            /// Useful checks for parts of a string
            bool starts_with(u8view str) const {
                return u8view{buffer.slice(0, str.buffer.size())} == str;
            }
            bool ends_with(u8view str) const {
                if (str.bytes() > bytes()) {
                    return false;
                } else if (str.bytes() == bytes()) {
                    return *this == str;
                } else {
                    return substr(code_points() - str.code_points()) == str;
                }
            }
        };

        /// ADL `std::size`and `std::data`  implementations
        inline auto size(u8view v) { return v.bytes(); }
        inline auto data(u8view v) { return v.data(); }

        /// Equality against other types
        inline bool operator==(lstring l, u8view r) { return r.operator==(l); }
        inline bool operator!=(lstring l, u8view r) { return r.operator!=(l); }
        template<std::size_t N>
        inline bool operator==(char const (&l)[N], u8view r) {
            return r.operator==(l);
        }
        template<std::size_t N>
        inline bool operator!=(char const (&l)[N], u8view r) {
            return r.operator!=(l);
        }
        inline bool operator==(const std::string &l, u8view r) {
            return r.operator==(l);
        }
        inline bool operator!=(const std::string &l, u8view r) {
            return r.operator!=(l);
        }

        /// Comparison against other types
        inline bool operator<(lstring l, u8view r) { return u8view(l) < r; }

        /// Concatenation
        inline std::string &operator+=(std::string &s, u8view e) {
            s.append(e.data(), e.bytes());
            return s;
        }


    }


    using u8view = cord::u8view;


}
