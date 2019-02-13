/**
    Copyright 2018-2019, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <atomic>
#include <memory>


namespace f5 {


    /// ## `control`
    /**
        Control block for owned memory.
     */
    struct control {
        /// Use a virtual destructor for type erasure
        virtual ~control() = default;

        /**
            Creates a new control block with an ownership count of 1.
         */
        template<typename S>
        static std::pair<std::unique_ptr<control>, S *> make(S &&s) {
            struct sub : public control {
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

        std::atomic<std::size_t> ownership_count = 1u;
    };


}
