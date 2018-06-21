/*
    Copyright 2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/memory.hpp>
#include <f5/cord/lstring.hpp>
#include <f5/cord/unicode-encodings.hpp>
#include <f5/cord/unicode-iterators.hpp>

#include <cstring>


namespace f5 {


    inline namespace cord {


        /// For unsigned char types with an UTF-8 encoding
        class u8view {
            const_u8buffer buffer;
        public:
            u8view() {}

            u8view(const_u8buffer b)
            : buffer(b) {
            }

            template<std::size_t N>
            u8view(const char (&s)[N])
            : buffer(reinterpret_cast<const unsigned char *>(s), N-1) {
            }

            u8view(const char *b, std::size_t s)
            : buffer(reinterpret_cast<const unsigned char *>(b), s) {
            }

            explicit u8view(const std::string &u8)
            : buffer(
                    reinterpret_cast<const unsigned char *>(u8.data()),
                    u8.size())
            {
            }

            u8view(lstring s)
            : buffer(reinterpret_cast<const unsigned char *>(s.c_str()), s.size()) {
            }

            /// An iterator that spits out UTF32 code points from the string
            class const_iterator : public std::iterator<
                    std::forward_iterator_tag,
                    utf32,
                    std::ptrdiff_t,
                    const utf32 *,
                    utf32>
            {
                friend class u8view;
                const_u8buffer buffer;

                const_iterator(const_u8buffer b)
                : buffer(b) {
                }
            public:
                const_iterator() {}

                utf32 operator * () const {
                    return decode_one(buffer).first;
                }
                const_iterator &operator ++ () {
                    const auto here = **this;
                    const auto bytes = u8length(here);
                    buffer = const_u8buffer(buffer.data() + bytes, buffer.size() - bytes);
                    return *this;
                }
                const_iterator operator ++ (int) {
                    const_iterator ret{*this};
                    ++(*this);
                    return ret;
                }

                const_iterator &operator += (std::size_t cps) {
                    while ( cps-- ) // Not undefined behaviour
                        ++(*this);
                    return *this;
                }

                bool operator == (const_iterator it) const {
                    return buffer.data() == it.buffer.data();
                }
                bool operator != (const_iterator it) const {
                    return buffer.data() != it.buffer.data();
                }
            };

            /// An iterator that produces UTF16 code points from the string
            using const_u16_iterator = f5::const_u32u16_iterator<const_iterator>;

            /// Construct a u8view from part of another
            u8view(const_iterator s, const_iterator e)
            : buffer(s.buffer.data(), s.buffer.size() - e.buffer.size()) {
            }

            /// Return the data array
            const char *data() const {
                return reinterpret_cast<const char *>(buffer.data());
            }
            /// Return the size in bytes of the string
            std::size_t bytes() const {
                return buffer.size();
            }
            /// Return true if the view is empty
            bool empty() const {
                return buffer.empty();
            }
            /// Return the underlying memory block for the data
            auto memory() const {
                return buffer;
            }

            /// Comparison. Acts as a string would. Not unicode aware in
            /// that it doesn't take into account normalisation, it only
            /// compares the byte values.
            bool operator == (u8view r) const {
                return bytes() == r.bytes() &&
                    std::memcmp(data(), r.data(), bytes()) == 0;
            }
            bool operator != (u8view r) const {
                return not ((*this) == r);
            }
            bool operator == (const char *s) const {
                std::size_t pos{};
                for ( ; pos < buffer.size() && *s; ++pos, ++s ) {
                    if ( buffer[pos] != (unsigned char)*s ) return false;
                }
                return pos == buffer.size() && *s == 0;
            }
            bool operator != (const char *s) const {
                return not ((*this) == s);
            }

            bool operator < (f5::u8view r) const {
                return buffer < r.buffer;
            }

            /// Safe substring against Unicode code point counts. The result
            /// undefined if the end marker is smaller than the start marker.
            u8view substr(std::size_t s) {
                auto pos = begin(), e = end();
                for ( ; s && pos != e; --s, ++pos );
                return u8view(pos, e);
            }
            u8view substr(std::size_t s, std::size_t e) {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return u8view(starts.data(), ends.data() - starts.data());
            }

            /// Return the begin iterator that delivers UTF32 code points
            const_iterator begin() const {
                return buffer;
            }
            /// Return the end iterator that delivers UTF32 code points
            const_iterator end() const {
                return const_u8buffer(buffer.data() + buffer.size(), std::size_t{0u});
            }

            /// Return the begin iterator that delivers UTF16 code points
            const_u16_iterator u16begin() const {
                return const_u16_iterator(begin(), end());
            }
            /// Return the end iterator that delivers UTF16 code points
            const_u16_iterator u16end() const {
                return const_u16_iterator(end(), end());
            }

            /// Safe conversions
            operator const_u8buffer () const {
                return buffer;
            }
            explicit operator std::string () const {
                return std::string(buffer.data(), buffer.data() + buffer.size());
            }
        };


        /// Concatenations with std::string
        inline std::string operator + (std::string f, u8view e) {
            f.append(e.data(), e.bytes());
            return f;
        }
        inline std::string operator + (u8view f, u8view e) {
            std::string r;
            r.reserve(f.bytes() + e.bytes());
            r.append(f.data(), f.bytes());
            r.append(e.data(), e.bytes());
            return r;
        }

        inline std::string &operator += (std::string &s, u8view e) {
            s.append(e.data(), e.bytes());
            return s;
        }


    }


}

