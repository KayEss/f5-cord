/**
    Copyright 2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <type_traits>


namespace f5 {


    namespace detail {
        template<
                typename Default,
                typename AlwaysVoid,
                template<typename...>
                typename Op,
                typename... Args>
        struct detector {
            using value_t = std::false_type;
            using type = Default;
        };
        template<typename Default, template<typename...> typename Op, typename... Args>
        struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
            using value_t = std::true_type;
            using type = Op<Args...>;
        };

        struct nonesuch {
            ~nonesuch() = delete;
            nonesuch(nonesuch const &) = delete;
            void operator=(nonesuch const &) = delete;
        };
    }


    template<template<typename...> typename Op, typename... Args>
    using is_detected = typename detail::
            detector<detail::nonesuch, void, Op, Args...>::value_t;
    template<template<class...> class Op, class... Args>
    constexpr bool is_detected_v = is_detected<Op, Args...>::value;

    template<template<typename...> typename Op, typename... Args>
    using detected_t =
            typename detail::detector<detail::nonesuch, void, Op, Args...>::type;

    template<typename Default, template<typename...> typename Op, typename... Args>
    using detected_or = detail::detector<Default, void, Op, Args...>;


    template<typename L, typename R>
    using op_eq_t =
            decltype(std::declval<L const &>() == std::declval<R const &>());


}
