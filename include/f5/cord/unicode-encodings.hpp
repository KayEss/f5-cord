/**
    Copyright 2016-2018, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/raise.hpp>
#include <f5/cord/unicode-core.hpp>
#include <array>
#include <stdexcept>
#include <cstdint>


namespace f5 {


    namespace cord {


        /// Check that the UTF32 code point is valid. Throw an exception if not.
        template<typename E = std::domain_error>
        constexpr inline bool check_valid(utf32 cp) {
            if (cp >= 0xd800 && cp <= 0xdbff) {
                raise<E>(
                        "UTF32 code point is in the leading UTF16 surrogate "
                        "pair range");
            } else if (cp >= 0xdc00 && cp <= 0xdfff) {
                raise<E>(
                        "UTF32 code point is in the trailing UTF16 surrogate "
                        "pair range");
            } else if (cp > 0x10ffff) {
                raise<E>("UTF32 code point is beyond the allowable range");
            } else {
                return true;
            }
            return false;
        }
        /// Return a valid code point (or the replacement character)
        template<typename E = std::domain_error>
        constexpr inline utf32 return_valid(utf32 cp) {
            if (cp >= 0xd800 && cp <= 0xdbff) {
                raise<E>(
                        "UTF32 code point is in the leading UTF16 surrogate "
                        "pair range");
            } else if (cp >= 0xdc00 && cp <= 0xdfff) {
                raise<E>(
                        "UTF32 code point is in the trailing UTF16 surrogate "
                        "pair range");
            } else if (cp > 0x10ffff) {
                raise<E>("UTF32 code point is beyond the allowable range");
            } else {
                return cp;
            }
            return 0xfffd;
        }

        /// Return the number of UTF8 values that this code point will
        /// occupy. If the code point falls in an invalid range then an
        /// exception will be thrown.
        template<typename E = std::domain_error>
        constexpr inline char u8length(utf32 cp) {
            if (not check_valid<E>(cp))
                return 0u;
            else if (cp < 0x00080)
                return 1u;
            else if (cp < 0x00800)
                return 2u;
            else if (cp < 0x10000)
                return 3u;
            else
                return 4u;
        }

        /// Return up to 4 bytes of the UTF8 sequence for a code point.
        /// If the code point needs less than four bytes then unused ones
        /// will have a value of zero. A zero in the first position is used
        /// if the input was zero and is part of the encoded sequence.
        template<typename E = std::domain_error>
        constexpr inline std::pair<char, std::array<utf8, 4>>
                u8encode(utf32 cp) {
            char length = u8length<E>(cp);
            switch (length) {
            case 1: return {1u, {{static_cast<utf8>(cp & 0x7f), 0u, 0u, 0u}}};
            case 2:
                return {2u,
                        {{static_cast<utf8>(0xc0 | ((cp >> 6) & 0x1f)),
                          static_cast<utf8>(0x80 | (cp & 0x3f)), 0u, 0u}}};
            case 3:
                return {3u,
                        {{static_cast<utf8>(0xe0 | ((cp >> 12) & 0xf)),
                          static_cast<utf8>(0x80 | ((cp >> 6) & 0x3f)),
                          static_cast<utf8>(0x80 | (cp & 0x3f)), 0u}}};
            case 4:
                return {4u,
                        {{static_cast<utf8>(0xf0 | ((cp >> 18) & 0x7)),
                          static_cast<utf8>(0x80 | ((cp >> 12) & 0x3f)),
                          static_cast<utf8>(0x80 | ((cp >> 6) & 0x3f)),
                          static_cast<utf8>(0x80 | (cp & 0x3f))}}};
            default:
                raise<E>("Cannot encode an invalid UTF32 code point ");
                return {0u, {{0u, 0u, 0u, 0u}}};
            }
        }


        /// Returns the number of bytes used to encode a UTF-8 sequence
        /// as implied by the first byte.
        template<typename E = std::domain_error>
        constexpr inline std::size_t u8codepoint_length(utf8 u8) {
            const auto ch{static_cast<unsigned char>(u8)};
            if (ch < 0x80)
                return 1u;
            else if (ch >= 0x80 && ch <= 0xBF)
                raise<E>("UTF8 continuation byte found in lead position");
            else if (ch >= 0xC0 && ch < 0xE0)
                return 2u;
            else if (ch >= 0xE0 && ch < 0xF0)
                return 3u;
            else if (ch >= 0xF0 && ch < 0xF8)
                return 4u;
            else if (ch >= 0xF8 && ch < 0xFC)
                raise<E>("UTF8 control byte may not imply five byte sequence");
            else if (ch >= 0xFC && ch < 0xFE)
                raise<E>("UTF8 control byte may not imply six byte sequence");
            else
                raise<E>("Invalid UTF8 byte value 0xFF");
            return 0u;
        }

        /// Decode a single UTF-8 code point from the memory buffer.
        /// Returns both the code point and the rest of the buffer.
        template<typename L = std::length_error, typename E = std::domain_error>
        constexpr inline std::pair<utf32, const_u8buffer>
                decode_one(const_u8buffer buffer) {
            if (not buffer.size()) {
                raise<L>("Can't decode UTF8 from an empty data buffer");
                return std::make_pair(0u, buffer);
            }
            auto first = buffer[0];
            auto bytes = u8codepoint_length<E>(first);
            if (buffer.size() < bytes) {
                raise<L>(
                        "Not enough characters in the buffer to decode a UTF8 "
                        "code point");
                return std::make_pair(0u, buffer);
            }
            utf32 ch{};
            switch (bytes) {
            case 1: ch = utf32(buffer[0] & 0x7F); break;
            case 2:
                ch = utf32(buffer[0] & 0x1F) << 6;
                ch |= utf32(buffer[1] & 0x3F);
                break;
            case 3:
                ch = utf32(buffer[0] & 0x0F) << 12;
                ch |= utf32(buffer[1] & 0x3F) << 6;
                ch |= utf32(buffer[2] & 0x3F);
                break;
            case 4:
                ch = utf32(buffer[0] & 0x07) << 18;
                ch |= utf32(buffer[1] & 0x3F) << 12;
                ch |= utf32(buffer[2] & 0x3F) << 6;
                ch |= utf32(buffer[3] & 0x3F);
                break;
            }
            return std::make_pair(ch, buffer.slice(bytes));
        }


        /// Return the number of UTF16 code points needed to encode
        /// the specified UTF32 code point.
        template<typename E = std::domain_error>
        constexpr inline char u16length(utf32 cp) {
            if (not check_valid(cp))
                return 0;
            else if (cp < 0x1'0000)
                return 1;
            else
                return 2;
        }


        /// Return either a single UTF16 code point or a surrogate pair
        /// that describes the UTF32 code point passed in
        template<typename E = std::domain_error>
        constexpr inline std::pair<char, std::array<utf16, 2>>
                u16encode(utf32 cp) {
            auto length = u16length(cp);
            if (length == 1) {
                return {1, {{static_cast<utf16>(cp), 0u}}};
            } else if (length == 0) {
                raise<E>("Cannot convert an invalid code point to UTF-16");
                return {0, {{0, 0}}};
            } else {
                return {2,
                        {{static_cast<utf16>(
                                  0xD800 - (0x10000 >> 10) + (cp >> 10)),
                          static_cast<utf16>(0xDC00 + (cp & 0x3FF))}}};
            }
        }


        /// Return true if this UTF-16 code represents either part of a
        /// surrogate pair
        constexpr inline bool is_surrogate(utf16 cp) {
            return cp >= 0xd800 && cp <= 0xdfff;
        }


        /// Decode a surrogate pair
        template<typename E = std::domain_error>
        constexpr inline utf32 u16decode(utf16 first, utf16 second) {
            return return_valid<E>(
                    (utf32(first) << 10) + second + 0x10000 - (0xD800 << 10)
                    - 0xDC00);
        }


    }


}
