/**
    Copyright 2018-2019, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/cord/unicode-view.hpp>

#ifndef assert
#include <cassert>
#endif


namespace f5 {


    namespace cord {


        /// UTF8 string with shared ownership.
        class u8string {
            friend class u8view;

            const_u8buffer buffer;
            using control_type = u8view::control_type;
            control_type *owner;

            /// Temporary re-allocation function used to handle the cases
            /// where the string might have been crated from a `u8view`
            /// that was created from a `std::string`.
            void transitional_allocation() {
                if (owner == nullptr) { shrink_to_fit(); }
            }

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
            : buffer{v.buffer}, owner{control_type::increment(v.owner)} {
                transitional_allocation();
            }

            /// From literals we have a `nullptr` control block as we have
            /// nothing to count
            u8string(lstring l) noexcept
            : buffer{l.data(), l.size()}, owner{} {}
            template<std::size_t N>
            u8string(const char (&a)[N]) noexcept : u8string{lstring{a}} {}

            /// For `std::string` we have to move the string into a memory area
            /// we can control
            explicit u8string(std::string s) : buffer{}, owner{} {
                auto created = control_type::make(std::move(s), s.size());
                owner = created.first.release();
                buffer = const_u8buffer{created.second->data(),
                                        created.second->size()};
            }

            ~u8string() { control_type::decrement(owner); }


            /// ## Conversions
            operator u8view() const { return u8view{buffer, owner}; }

            explicit operator const_u8buffer() const { return buffer; }
            explicit operator f5::buffer<byte const>() const {
                return static_cast<f5::buffer<byte const>>(
                        static_cast<u8view>(*this));
            }
            explicit operator std::string_view() const noexcept {
                return static_cast<std::string_view>(
                        static_cast<u8view>(*this));
            }
            explicit operator std::string() const {
                return static_cast<std::string>(static_cast<u8view>(*this));
            }

            /// Force re-allocation of the memory such that we also have
            /// a NUL at the end of string. This guarantees that the string
            /// is now also C safe. We make this operation idempotent by
            /// storing the number of bytes used in the original string
            /// allocation so that we can check if we need to re-allocate s
            /// new string or not.
            char const *shrink_to_fit() {
                if ((owner && owner->user_data != bytes()) || not owner) {
                    *this = u8string{static_cast<std::string>(*this)};
                }
                return data();
            }

            /// ## Assignment
            u8string &operator=(const u8string &s) noexcept {
                control_type::decrement(owner);
                buffer = s.buffer;
                owner = control_type::increment(s.owner);
                return *this;
            }
            u8string &operator=(u8string &&s) noexcept {
                control_type::decrement(owner);
                buffer = s.buffer;
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

            /// Construct from a pair of iterators
            u8string(const_iterator b, const_iterator e) noexcept
            : buffer{b.buffer.data(), b.buffer.size() - e.buffer.size()},
              owner(control_type::increment(b.owner)) {
                assert(b.owner == e.owner);
                transitional_allocation();
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

            /// Returns `true` if this is a shared string
            bool is_shared() const noexcept { return owner != nullptr; }
            /// Returns true if the other string uses the same allocation
            /// as this (they have the same control block).
            bool shares_allocation_with(u8view v) noexcept {
                return owner != nullptr && owner == v.owner;
            }

            /// Return the data array
            char const *data() const noexcept { return buffer.data(); }
            /// Return the size in bytes of the string
            std::size_t bytes() const noexcept { return buffer.size(); }
            /// Return the size in code points
            auto code_points() const { return std::distance(begin(), end()); }
            /// Return true if the string is empty
            bool empty() const noexcept { return buffer.empty(); }
            /// Return the underlying memory block for the data
            auto memory() const noexcept { return u8view{*this}.memory(); }

            /// Useful checks for parts of a string
            bool starts_with(u8view str) const {
                return u8view{buffer.slice(0, str.bytes())} == str;
            }
            bool ends_with(u8view str) const {
                return u8view{*this}.ends_with(str);
            }


            /// ## Substrings

            /// Safe substring against Unicode code point counts. The result
            /// is undefined if the end marker is smaller than the start marker.
            u8string substr(std::size_t s) const {
                auto pos = begin(), e = end();
                for (; s && pos != e; --s, ++pos)
                    ;
                return u8string(pos, e);
            }
            u8string substr_pos(std::size_t s, std::size_t e) const {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return u8string(starts.begin(), ends.begin());
            }
            [[deprecated("Use substr_pos")]] auto
                    substr(std::size_t s, std::size_t e) const {
                return substr_pos(s, e);
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
        };


        template<std::size_t N>
        inline bool operator==(char const (&l)[N], const u8string &r) {
            return r == l;
        }
        inline bool operator==(const std::string &l, const u8string &r) {
            return u8view{r} == l;
        }


        /// ## Concatenation
        inline u8string operator+(u8view f, u8view e) {
            std::string r;
            r.reserve(f.bytes() + e.bytes());
            r.append(f.data(), f.bytes());
            r.append(e.data(), e.bytes());
            return u8string{r};
        }
        template<std::size_t N>
        inline u8string operator+(u8view f, char const (&e)[N]) {
            return operator+(f, u8view{e});
        }
        template<std::size_t N>
        inline u8string operator+(const u8string &f, char const (&e)[N]) {
            return operator+(u8view{f}, u8view{e});
        }
        inline u8string operator+(u8view f, const std::string &e) {
            return operator+(f, u8view{e.data(), e.size()});
        }
        inline u8string operator+(u8string f, const std::string &e) {
            return operator+(u8view{f}, u8view{e.data(), e.size()});
        }
        inline u8string operator+(const std::string &f, u8view e) {
            return operator+(u8view{f.data(), f.size()}, e);
        }
        template<std::size_t N>
        inline u8string operator+(char const (&f)[N], u8view e) {
            return operator+(u8view{f}, e);
        }


    }


    using u8string = cord::u8string;


}
