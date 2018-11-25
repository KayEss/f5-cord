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

            explicit u8string(u8shared b) noexcept : buffer{std::move(b)} {}

            explicit u8string(lstring l)
            : buffer{std::shared_ptr<const unsigned char>{
                             reinterpret_cast<const unsigned char *>(l.data()),
                             [](auto &&) {}},
                     l.size()} {}
            template<std::size_t N>
            u8string(const char (&a)[N]) : u8string{lstring{a}} {}


            /// ## Iteration

            /// An iterator that produces UTF32 code points
            using const_iterator = const_u32_iterator<u8shared>;
            const_iterator begin() const noexcept {
                return const_iterator{buffer};
            }
            const_iterator end() const noexcept {
                return const_iterator{buffer.slice(buffer.size())};
            }
            u8string(const_iterator b, const_iterator e) noexcept
            : buffer{b.buffer, b.buffer.data(),
                     b.buffer.size() - e.buffer.size()} {}

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
            std::size_t bytes() const noexcept { return buffer.size(); }
            bool empty() const noexcept { return bytes() == 0; }
            /// Return the underlying memory block for the data
            auto memory() const noexcept { return buffer; }

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
            explicit operator u8shared() const noexcept { return buffer; }
            operator u8view() const noexcept {
                return u8view{const_u8buffer{buffer}};
            }
            explicit operator std::string_view() const noexcept {
                return static_cast<std::string_view>(
                        static_cast<u8view>(*this));
            }
        };


    }


}
