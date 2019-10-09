/**
    Copyright 2018-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <atomic>
#include <iterator>
#include <memory>

#ifndef assert
#include <cassert>
#endif


namespace f5 {


    /// ## `control`
    /**
        Control block for owned memory.
     */
    template<typename T = void>
    struct control;


    /**
        For the `void` superclass we don't store any extra information in
        the control block for the client code.
     */
    template<>
    struct control<void> {
        /// Use a virtual destructor for type erasure
        virtual ~control() = default;

        /**
            Creates a new control block with an ownership count of 1.
         */
        template<typename S>
        static std::pair<std::unique_ptr<control<void>>, S *> make(S &&s) {
            struct sub : public control<void> {
                S item;
                sub(S &&s) : item{std::move(s)} {}
            };
            auto made = std::make_unique<sub>(std::move(s));
            return {std::move(made), &made->item};
        }

        /**
            Increment and decrement the usage count. We never need to do
           anything after the increment so we don't need to know the exact
           count. Decrementing is another matter though as we have to get the
           exact right count for zero as we will destruct the control block at
           that point (which in turn will destruct the owned memory.
         */
        static control *increment(control *c) noexcept {
            if (c) ++c->ownership_count;
            return c;
        }
        static void decrement(control *c) noexcept {
            if (c && --c->ownership_count == 0u) { delete c; }
        }

      private:
        std::atomic<std::size_t> ownership_count = 1u;
    };


    /**
        If client code needs to store extra information in the control block
        then this type can be used.
     */
    template<typename T>
    struct control : private control<void> {
        T user_data;
        control(T t) : user_data{std::move(t)} {}

        static control *increment(control *c) noexcept {
            control<void>::increment(c);
            return c;
        }
        static void decrement(control *c) noexcept {
            control<void>::decrement(c);
        }

        /**
            Creates a new control block with an ownership count of 1.
         */
        template<typename S>
        static std::pair<std::unique_ptr<control<T>>, S *> make(S &&s, T t) {
            struct sub : public control<T> {
                S item;
                sub(S &&s, T t)
                : control<T>{std::move(t)}, item{std::move(s)} {}
            };
            auto made = std::make_unique<sub>(std::move(s), std::move(t));
            return {std::move(made), &made->item};
        }
    };


    /**
        This type can be used to wrap an iterator so that it will also carry
        a pointer to a control block.
     */


    template<typename Iter, typename Control>
    struct owner_tracking_iterator {
        using difference_type =
                typename std::iterator_traits<Iter>::difference_type;
        using value_type = typename std::iterator_traits<Iter>::value_type;
        using pointer = typename std::iterator_traits<Iter>::pointer;
        using reference = typename std::iterator_traits<Iter>::reference;
        using iterator_category =
                typename std::iterator_traits<Iter>::iterator_category;

        using iterator_type = Iter;
        using control_type = std::add_pointer_t<Control>;


        constexpr owner_tracking_iterator() : iterator{}, owner{} {}

        constexpr owner_tracking_iterator(
                Iter i, control_type c = nullptr) noexcept
        : iterator{i}, owner{c} {}

        constexpr decltype(auto) operator*() const { return *iterator; }
        constexpr owner_tracking_iterator &operator++() {
            ++iterator;
            return *this;
        }
        constexpr owner_tracking_iterator operator++(int) {
            auto ret = *this;
            iterator++;
            return ret;
        }
        constexpr bool operator==(owner_tracking_iterator i) const noexcept {
            assert(owner == i.owner);
            return iterator == i.iterator;
        }
        constexpr bool operator!=(owner_tracking_iterator i) const noexcept {
            assert(owner == i.owner);
            return iterator != i.iterator;
        }

        decltype(auto) operator-(const owner_tracking_iterator &it) {
            return iterator - it.iterator;
        }

        Iter iterator;
        control_type owner;
    };


}
