/**
    Copyright 2016-2018, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/cord/lstring.hpp>


namespace f5 {


    namespace cord {


        /// A compile time string which is a concatenation of other compile
        /// time strings.
        [[deprecated("Prefer the use of tstring")]] template<std::size_t N>
        struct vstring {
            lstring strings[N];

            constexpr lstring &operator[](std::size_t i) {
                return i < N ? strings[i]
                             : throw std::out_of_range(
                                       "Indexing past end of vstring");
            }

            operator std::string() const {
                std::string result;
                for (std::size_t i{0}; i < N; ++i) {
                    result += strings[i].c_str();
                }
                return result;
            }
        };


        /// Allow compile time concatenation of strings.
        constexpr auto operator+(lstring p, lstring s) {
            return vstring<2>{p, s};
        }
        /// Allow compile time concatenation of strings.
        template<std::size_t N>
        constexpr auto operator+(vstring<N> p, lstring s) {
            vstring<N + 1> result;
            for (std::size_t i{0}; i != N; ++i) { result[i] = p[i]; }
            result[N] = s;
            return result;
        }
        /// Allow compile time concatenation of strings.
        template<std::size_t N>
        constexpr auto operator+(lstring p, vstring<N> s) {
            vstring<N + 1> result;
            result[0] = p;
            for (std::size_t i{0}; i != N; ++i) { result[i + 1] = s[i]; }
            return result;
        }


        /// Allow run time concatenation of strings.
        template<std::size_t N>
        inline auto operator+(vstring<N> l, const std::string &r) {
            return std::string(l) + r;
        }
        /// Allow run time concatenation of strings.
        template<std::size_t N>
        inline std::string &&operator+(std::string &&l, vstring<N> r) {
            for (std::size_t i{0}; i != N; ++i) { l += r[i].c_str(); }
            return std::move(l);
        }


    }


}
