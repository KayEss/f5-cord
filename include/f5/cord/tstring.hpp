/**
    Copyright 2017-2018, Felspar Co Ltd. <https://kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/memory.hpp>
#include <f5/cord/lstring.hpp>
#include <f5/cord/unicode-core.hpp>


namespace f5 {


    namespace cord {


        /// A compile time string where the string itself is to be manipulated
        template<char... Text>
        struct tstring {
            static constexpr char bytes[sizeof...(Text) + 1] = {Text..., 0};
            static constexpr lstring as_lstring() { return lstring(bytes); }

            constexpr tstring() {}

            constexpr std::size_t size() const { return sizeof...(Text); }

            template<char... App>
            constexpr auto operator+(tstring<App...>) const {
                return tstring<Text..., App...>();
            }

            std::string as_string() const { return std::string{Text...}; }
            /// Allow conversion to a std::string (should this be explicit?)
            operator std::string() const { return as_string(); }
            /// Safe conversions
            operator lstring() const { return as_lstring(); }
            operator const_u8buffer() const { return as_lstring(); }
        };

        template<char... Text>
        constexpr char tstring<Text...>::bytes[sizeof...(Text) + 1];

        template<char... Text>
        constexpr inline bool operator==(tstring<Text...> l, lstring r) {
            return l.as_lstring() == r;
        }
        template<char... Text>
        constexpr inline bool operator==(tstring<Text...>, tstring<Text...>) {
            return true;
        }
        template<char... Text1, char... Text2>
        constexpr inline bool operator==(tstring<Text1...>, tstring<Text2...>) {
            return false;
        }
        template<char... Text>
        constexpr inline bool operator!=(tstring<Text...>, tstring<Text...>) {
            return false;
        }
        template<char... Text1, char... Text2>
        constexpr inline bool operator!=(tstring<Text1...>, tstring<Text2...>) {
            return true;
        }


    }


    inline namespace literals {


        template<typename C, C... Text>
        constexpr inline auto operator"" _t() {
            static_assert(
                    sizeof(C) == 1,
                    "Only char types allowed with _t string literals");
            return cord::tstring<Text...>{};
        }


    }


}
