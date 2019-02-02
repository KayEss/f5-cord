/**
    Copyright 2016-2018, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/cord/unicode-encodings.hpp>

#include <tuple>


namespace f5 {


    namespace cord {


        /// An iterator that produces UTF-32 from a UTF-16 iterator
        template<typename U16, typename E = std::range_error>
        class const_u16u32_iterator :
        public std::iterator<
                std::forward_iterator_tag,
                utf32,
                typename U16::difference_type,
                const utf32 *,
                utf32> {
            U16 pos, end;

          public:
            /// Default construct
            const_u16u32_iterator() : pos{}, end{} {}

            /// Construct from a begin and end pair
            const_u16u32_iterator(U16 begin, U16 end) : pos(begin), end(end) {}

            /// Increment position
            const_u16u32_iterator &operator++() {
                if (u16length(**this) == 2) ++pos;
                ++pos;
                return *this;
            }

            /// Return the current character
            utf32 operator*() const {
                if (is_surrogate(*pos)) {
                    auto next = pos;
                    ++next;
                    if (next == end) {
                        raise<E>("Truncated surrogate pair in UTF-16 sequence");
                        return 0;
                    } else {
                        return u16decode(*pos, *next);
                    }
                } else {
                    return *pos;
                }
            }

            /// Compare for equality
            bool operator==(const const_u16u32_iterator &rhs) const {
                return pos == rhs.pos;
            }
            /// Compare for inequality
            bool operator!=(const const_u16u32_iterator &rhs) const {
                return pos != rhs.pos;
            }
        };


        /// Make a pair of iterators from a UTF-32 iterator
        template<typename E = std::range_error, typename Iterator>
        inline auto make_u16u32_iterator(Iterator b, Iterator e) {
            return std::make_pair(
                    const_u16u32_iterator<Iterator, E>(b, e),
                    const_u16u32_iterator<Iterator, E>(e, e));
        }


        /// An iterator that produces UTF-16 from a UTF-32 iterator
        template<typename U32, typename E = std::range_error>
        class const_u32u16_iterator :
        public std::iterator<
                std::forward_iterator_tag,
                utf16,
                typename U32::difference_type,
                const utf16 *,
                utf16> {
            U32 pos, end;
            std::array<utf16, 2> buffer;
            char in_buffer;

            /// Decode the current position (if safe)
            void decode() {
                if (pos != end) {
                    std::tie(in_buffer, buffer) = f5::cord::u16encode(*pos);
                }
            }

          public:
            /// Default construct
            const_u32u16_iterator() : in_buffer{} {}

            /// Wrap a U32 iterator
            const_u32u16_iterator(U32 pos, U32 end)
            : pos(pos), end(end), in_buffer{} {
                decode();
            }

            /// The current code point
            utf16 operator*() const { return buffer[0]; }

            /// Increment the iterator
            const_u32u16_iterator &operator++() {
                if (in_buffer) {
                    /// If there is anything in the local buffer then
                    /// we can decrement its count and shift down
                    /// the buffer content. This is always safe
                    --in_buffer;
                    buffer[0] = buffer[1];
                }
                if (not in_buffer && pos != end) {
                    /// Our local buffer is empty and we're not at
                    /// the end of the sequence, so we can move
                    /// to the next position and decode it
                    ++pos;
                    decode();
                } else if (pos == end) {
                    raise<E>("Run off the end of the iterator");
                }
                return *this;
            }

            /// Post increment the iterator
            const_u32u16_iterator operator++(int) {
                auto ret = *this;
                ++(*this);
                return ret;
            }

            /// Compare iterators
            bool operator==(const const_u32u16_iterator &rhs) const {
                return pos == rhs.pos && in_buffer == rhs.in_buffer;
            }
            /// Compare iterators
            bool operator!=(const const_u32u16_iterator &rhs) const {
                return not(*this == rhs);
            }

            /// Return a copy of the current position in the UTF-32 sequence
            const U32 &u32_iterator() const { return pos; }
        };


        /// Make a pair of iterators from a UTF-32 iterator
        template<typename Iterator, typename E = std::range_error>
        inline auto make_u32u16_iterator(Iterator b, Iterator e) {
            return std::make_pair(
                    const_u32u16_iterator<Iterator, E>(b, e),
                    const_u32u16_iterator<Iterator, E>(e, e));
        }


        template<typename B, typename C = void>
        struct const_u32_iterator :
        public std::iterator<
                std::forward_iterator_tag,
                utf32,
                std::ptrdiff_t,
                const utf32 *,
                utf32> {
            using buffer_type = B;
            using control_type = std::add_pointer_t<C>;

            buffer_type buffer;
            control_type owner;

            constexpr const_u32_iterator(control_type c = nullptr) : owner{c} {}

            constexpr explicit const_u32_iterator(
                    buffer_type b, control_type c = nullptr) noexcept
            : buffer(std::move(b)), owner{c} {}

            utf32 operator*() const { return decode_one(buffer).first; }
            const_u32_iterator &operator++() {
                const auto here = **this;
                const auto bytes = u8length(here);
                buffer = buffer.slice(bytes);
                return *this;
            }
            const_u32_iterator operator++(int) {
                const_u32_iterator ret{*this};
                ++(*this);
                return ret;
            }

            bool operator==(const_u32_iterator it) const noexcept {
                return buffer.data() == it.buffer.data();
            }
            bool operator!=(const_u32_iterator it) const noexcept {
                return buffer.data() != it.buffer.data();
            }
        };


    }


    template<typename U32, typename E = std::range_error>
    using const_u32u16_iterator [[deprecated(
            "Use the type in the f5::cord namespace, not the f5 namespace")]] =
            cord::const_u32u16_iterator<U32, E>;


    template<typename Iterator, typename E = std::range_error>
    [
            [deprecated("Use the function in the f5::cord namespace not the "
                        "f5::namespace")]] inline auto
            make_u32u16_iterator(Iterator b, Iterator e) {
        return cord::make_u32u16_iterator(b, e);
    }


}
