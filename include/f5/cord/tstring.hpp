/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/cord/lstring.hpp>


namespace f5 {


    inline namespace cord {


        /// A compile time string where the string itself is to be manipulated
        template<char ... Text>
        struct tstring {
            static constexpr char bytes[sizeof...(Text) + 1] = {Text..., 0};
            static constexpr lstring as_lstring() {
                return lstring(bytes);
            }

            constexpr tstring() {}

            constexpr std::size_t size() const {
                return sizeof...(Text);
            }

            template<char ... App>
            constexpr auto operator + (tstring<App...>) const {
                return tstring<Text..., App...>();
            }

            std::string as_string() const {
                return std::string{Text...};
            }
            operator std::string () const {
                return as_string();
            }
        };

        template<char ... Text>
        constexpr char tstring<Text...>::bytes[sizeof...(Text) + 1];

        template<char ... Text>
        constexpr bool operator == (tstring<Text...> l, lstring r) {
            return l.as_lstring() == r;
        }


    }


}

