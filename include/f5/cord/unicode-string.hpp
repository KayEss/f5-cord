/**
    Copyright 2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <iostream>
#include <f5/cord/unicode-view.hpp>


namespace f5 {


    inline namespace cord {


        /// UTF8 string with shared ownership.
        class u8string {
            friend class u8view;

            const_u8buffer buffer;
            using control_type = control<char const>;
            control_type *owner;

            u8string(const_u8buffer b, control_type *o)
            : buffer{b}, owner{control_type::increment(o)} {}

          public:
            /// ## Constructors
            u8string() noexcept : buffer{}, owner{} {}

            /// The type is copyable and movable. Handle the control block
            /// appropriately.
            u8string(const u8string &b)
            : buffer{b.buffer}, owner{control_type::increment(b.owner)} {}
            u8string(u8string &&b)
            : buffer{b.buffer}, owner{std::exchange(b.owner, nullptr)} {}

            /// Creation from a `u8view` will never allocate because the
            /// `u8view` remembers the shared status of its history
            u8string(u8view v)
            : buffer{v.buffer}, owner{control_type::increment(v.owner)} {}

            /// From literals we have a `nullptr` control block as we have
            /// nothing to count
            explicit u8string(lstring l) noexcept
            : buffer{l.data(), l.size()}, owner{} {}
            template<std::size_t N>
            u8string(const char (&a)[N]) noexcept : u8string{lstring{a}} {}

            /// For `std::string` we have to move the string into a memory area
            /// we can control
            explicit u8string(std::string s) : buffer{}, owner{} {
                // TODO This allocates memory twice. Once below for the
                // std::string, and once again in control_type::make for the
                // control block. We should be able to allocate all of this
                // memory in one go
                auto ss = std::make_unique<std::string>(std::move(s));
                auto const sp = ss->data();
                buffer = const_u8buffer{sp, ss->size()};
                owner = control_type::make(
                        sp, [ss = ss.release()](auto const *) { delete ss; });
            }

            ~u8string() { control_type::decrement(owner); }


            /// ## Assignment
            u8string &operator=(const u8string &s) {
                buffer = s.buffer;
                control_type::decrement(owner);
                owner = control_type::increment(s.owner);
                return *this;
            }
            u8string &operator=(u8string &&s) {
                buffer = s.buffer;
                control_type::decrement(owner);
                owner = std::exchange(s.owner, nullptr);
                return *this;
            }

            /// ## Iteration

            /// An iterator that produces UTF32 code points
            using const_iterator =
                    const_u32_iterator<const_u8buffer, control_type>;
            const_iterator begin() const noexcept {
                return const_iterator{buffer, owner};
            }
            const_iterator end() const noexcept {
                return const_iterator{buffer.slice(buffer.size()), owner};
            }
            u8string(const_iterator b, const_iterator e) noexcept
            : buffer{b.buffer.data(), b.buffer.size() - e.buffer.size()},
              owner(control_type::increment(b.owner)) {}

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

            /// Returns `true` if this is a shared string
            bool is_shared() const { return owner != nullptr; }

            std::size_t bytes() const noexcept { return buffer.size(); }
            bool empty() const noexcept { return bytes() == 0; }
            /// Return the underlying memory block for the data
            auto memory() const noexcept { return buffer; }
            const char *data() const noexcept { return buffer.data(); }

            /// Useful checks for parts of a string
            bool starts_with(u8view str) const {
                return u8view{buffer.slice(0, str.bytes())} == str;
            }


            /// ## Substrings
            u8string substr(std::size_t s) const {
                auto pos = begin(), e = end();
                for (; s && pos != e; --s, ++pos)
                    ;
                return u8string(pos, e);
            }
            u8string substr(std::size_t s, std::size_t e) const {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return u8string(starts.begin(), ends.begin());
            }


            /// ## Comparisons
            /// Comparison. Acts as a string would. Not unicode aware in
            /// that it doesn't take into account normalisation, it only
            /// compares the byte values.
            bool operator==(u8view l) const {
                return u8view{const_u8buffer{buffer}} == l;
            }
            bool operator!=(u8view l) const {
                return u8view{const_u8buffer{buffer}} != l;
            }
            bool operator<(u8view l) const {
                return u8view{const_u8buffer{buffer}} < l;
            }
            bool operator<=(u8view l) const {
                return u8view{const_u8buffer{buffer}} <= l;
            }
            bool operator>=(u8view l) const {
                return u8view{const_u8buffer{buffer}} >= l;
            }
            bool operator>(u8view l) const {
                return u8view{const_u8buffer{buffer}} > l;
            }


            /// ## Conversions
            operator u8view() const noexcept { return u8view{buffer, owner}; }
            explicit operator std::string_view() const noexcept {
                return static_cast<std::string_view>(
                        static_cast<u8view>(*this));
            }
        };


    }


}
