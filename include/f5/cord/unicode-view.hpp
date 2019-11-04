/**
    Copyright 2017-2019 Red Anchor Trading Co. Ltd.

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


        /// String views for any Unicode code unit type
        template<
                typename C,
                typename E = std::range_error,
                typename IM = iterators<C, E>>
        class basic_view {
            f5::buffer<const C> buffer;
            control<std::size_t> *owner = nullptr;

          public:
            /// ## Types

            /// Buffer used to hold data (via the `owner`/`control_type`)
            using buffer_type = decltype(buffer);
            /// The memory control block type
            using control_type = std::remove_pointer_t<decltype(owner)>;
            /// The character type. Always constant
            using value_type = typename buffer_type::value_type;
            /// Iterator type map
            using iterator_map = IM;
            /// The standard string types
            using std_string =
                    std::basic_string<typename iterator_map::value_type>;
            using std_string_view =
                    std::basic_string_view<typename iterator_map::value_type>;
            /// Error exception type
            using encoding_error_type = E;


            /// ## Constructors

            constexpr basic_view() noexcept : buffer{}, owner{} {}

            constexpr explicit basic_view(buffer_type b) noexcept
            : buffer(b), owner{} {}

            template<std::size_t N>
            constexpr basic_view(value_type (&s)[N]) noexcept
            : buffer(s, N - 1), owner{} {}

            constexpr explicit basic_view(value_type *b, std::size_t s) noexcept
            : buffer(b, s), owner{} {}

            constexpr basic_view(lstring s) noexcept
            : buffer(s.data(), s.size()), owner{} {}

            /// This constructor is only meant to be used by the string type
            constexpr basic_view(decltype(buffer) b, decltype(owner) o) noexcept
            : buffer{b}, owner{o} {}


            /// ## Conversions
            constexpr operator std_string_view() const noexcept {
                return std_string_view(buffer.data(), buffer.size());
            }

            constexpr explicit operator buffer_type() const noexcept {
                return buffer;
            }
            explicit operator f5::buffer<byte const>() const noexcept {
                return {reinterpret_cast<byte const *>(buffer.data()),
                        buffer.size() * sizeof(value_type)};
            }
            explicit operator std_string() const {
                return std_string(buffer.data(), buffer.data() + buffer.size());
            }


            /// ## Iteration

            /// An iterator that spits out UTF32 code points from the string
            using const_iterator = typename iterator_map::
                    template u32iter<buffer_type, control_type>;

            /// Return the begin iterator that delivers UTF32 code points
            constexpr const_iterator begin() const {
                return IM::template make_iterator<buffer_type, control_type>(
                        buffer, owner);
            }
            /// Return the end iterator that delivers UTF32 code points
            constexpr const_iterator end() const {
                return IM::template make_iterator<buffer_type, control_type>(
                        buffer.slice(buffer.size()), owner);
            }

            /// Construct a basic_view from part of another
            constexpr basic_view(const_iterator s, const_iterator e) noexcept
            : buffer{IM::template get_buffer<buffer_type, control_type>(s, e)},
              owner{s.owner} {
                assert(s.owner == e.owner);
            }

            /// An iterator that produces UTF16 code units from the string
            using const_u16_iterator = typename iterator_map::
                    template u16iter<buffer_type, control_type>;

            /// Return the begin iterator that delivers UTF16 code units
            constexpr const_u16_iterator u16begin() const {
                return IM::template make_u16iterator<buffer_type, control_type>(
                        begin(), end());
            }
            /// Return the end iterator that delivers UTF16 code units
            constexpr const_u16_iterator u16end() const {
                return IM::template make_u16iterator<buffer_type, control_type>(
                        end(), end());
            }


            /// ## Queries

            /// Returns `true` if this view is over a `u8string` and is a shared
            /// string
            bool is_shared() const noexcept { return owner != nullptr; }
            /// Returns true if the other string uses the same allocation
            /// as this (they have the same control block).
            bool shares_allocation_with(basic_view v) noexcept {
                return owner != nullptr && owner == v.owner;
            }
            /// Return the memory control block
            control_type *control_block() const noexcept { return owner; }

            /// Return the data array
            constexpr value_type *data() const noexcept {
                return buffer.data();
            }
            /// Return the size in bytes of the string
            constexpr std::size_t bytes() const noexcept {
                return buffer.size() * sizeof(value_type);
            }
            /// Return the size in code units
            constexpr std::size_t code_units() const noexcept {
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
            basic_view substr(std::size_t s) const {
                auto pos = begin(), e = end();
                for (; s && pos != e; --s, ++pos)
                    ;
                return basic_view(pos, e);
            }
            basic_view substr_pos(std::size_t s, std::size_t e) const {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return basic_view{starts.begin(), ends.begin()};
            }
            [[deprecated("Use substr_pos")]] auto
                    substr(std::size_t s, std::size_t e) const {
                return substr_pos(s, e);
            }


            /// ## Comparisons

            /// Comparison. Acts as a string would. Not Unicode aware in
            /// that it doesn't take into account normalisation, it only
            /// compares the byte values.
            constexpr bool operator==(basic_view r) const noexcept {
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
            constexpr bool operator!=(basic_view r) const noexcept {
                return not((*this) == r);
            }
            bool operator==(const std_string &r) const noexcept {
                return *this == basic_view{r.data(), r.size()};
            }
            bool operator!=(const std_string &r) const noexcept {
                return *this != basic_view{r.data(), r.size()};
            }
            constexpr bool operator==(value_type *s) const noexcept {
                std::size_t pos{};
                for (; pos < buffer.size() && *s; ++pos, ++s) {
                    if (buffer[pos] != *s) return false;
                }
                return pos == buffer.size() && *s == 0;
            }
            constexpr bool operator!=(value_type *s) const noexcept {
                return not((*this) == s);
            }

            constexpr bool operator<(basic_view r) const {
                return buffer < r.buffer;
            }
            constexpr bool operator<=(basic_view r) const {
                return buffer <= r.buffer;
            }
            constexpr bool operator>=(basic_view r) const {
                return buffer >= r.buffer;
            }
            constexpr bool operator>(basic_view r) const {
                return buffer > r.buffer;
            }

            /// Useful checks for parts of a string
            bool starts_with(basic_view str) const {
                return basic_view{buffer.slice(0, str.buffer.size())} == str;
            }
            bool ends_with(basic_view str) const {
                if (str.code_units() > code_units()) {
                    return false;
                } else if (str.code_units() == code_units()) {
                    return *this == str;
                } else {
                    return substr(code_points() - str.code_points()) == str;
                }
            }
        };

        /// ## String view types
        using u8view = basic_view<char>;
        using u16view = basic_view<char16_t>;
        using u32view = basic_view<char32_t>;


        /// ADL `std::size`and `std::data`  implementations
        template<typename C>
        inline constexpr auto size(basic_view<C> const v) {
            return v.code_units();
        }
        template<typename C>
        inline constexpr auto data(basic_view<C> const v) {
            return v.data();
        }

        /// Equality against other types
        inline bool operator==(lstring l, u8view r) { return r.operator==(l); }
        inline bool operator!=(lstring l, u8view r) { return r.operator!=(l); }
        template<std::size_t N, typename C>
        inline constexpr bool
                operator==(C const (&l)[N], basic_view<C> const r) {
            return r.operator==(l);
        }
        template<std::size_t N, typename C>
        inline constexpr bool
                operator!=(C const (&l)[N], basic_view<C> const r) {
            return r.operator!=(l);
        }
        template<typename C>
        inline bool operator==(
                std::basic_string<C> const &l, basic_view<C> const r) {
            return r.operator==(l);
        }
        template<typename C>
        inline bool operator!=(
                std::basic_string<C> const &l, basic_view<C> const r) {
            return r.operator!=(l);
        }

        /// Comparison against other types
        inline bool operator<(lstring l, u8view r) { return u8view(l) < r; }


    }


    /// Aliases directly in `f5` as these will be used quite often
    using u8view = cord::u8view;
    using u16view = cord::u16view;
    using u32view = cord::u32view;


}
