/**
    Copyright 2018, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <atomic>
#include <functional>


namespace f5 {


    /// ## `control`
    /**
        Control block for owned memory.
     */
    template<typename T>
    struct control {
        using pointer_type = std::add_pointer_t<T>;
        using destructor_type = std::function<void(pointer_type)>;

        /**
            Creates a new control block with an ownership count of 1.
         */
        static control *make(pointer_type p, destructor_type d) {
            return new control(p, std::move(d));
        }

        /**
            Increment and decrement the usage count. We never need to do
           anything after the increment so we don't need to know the exact
           count. Decrementing is another matter though as we have to get the
           exact right count for zero as we will destruct the control block at
           that point (which in turn will destruct the owned memory.
         */
        static control *increment(control *c) {
            if (c) ++c->ownership_count;
            return c;
        }
        static void decrement(control *c) {
            if (c && --c->ownership_count == 0u) {
                c->destructor(c->owned_memory);
                delete c;
            }
        }

        control(pointer_type p, destructor_type d) noexcept
        : owned_memory{p}, destructor{std::move(d)} {}

        pointer_type owned_memory;
        destructor_type destructor;
        std::atomic<std::size_t> ownership_count = 1u;
    };


}
