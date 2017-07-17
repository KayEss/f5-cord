/*
    Copyright 2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <array>
#include <type_traits>
#include <vector>


namespace f5 {


    /// Basic view of contiguous blocks of some type. Two views
    /// are the same only if they point to the same underlying memory.
    template<typename V>
    class array_view final {
        V *m_data;
        std::size_t m_size;
    public:
        /// The value type
        using value_type = V;
        /// The underlying pointer type
        using pointer_type = std::add_pointer_t<std::remove_const_t<V>>;
        /// Pointer to a const version of the underlying type
        using pointer_const_type = std::add_pointer_t<std::add_const_t<V>>;

        /// Default construct an empty buffer
        array_view()
        : m_data(nullptr), m_size(0u) {
        }

        /// Construct from a vector
        array_view(std::vector<value_type> &v)
        : m_data(v.data()), m_size(v.size()) {
        }
        template<typename T>
        array_view(const std::vector<T> &v)
        : m_data(v.data()), m_size(v.size()) {
        }
        // For C++ arrays
        template<typename T, std::size_t N>
        constexpr array_view(std::array<T, N> &v)
        : m_data(v.data()), m_size(N) {
        }
        template<typename T, std::size_t N>
        constexpr array_view(const std::array<T, N> &v)
        : m_data(v.data()), m_size(N) {
        }
        /// From a C array
        template<std::size_t N>
        constexpr array_view(V (&a)[N])
        : m_data(a), m_size(N) {
        }

        /// Construct from pointers
        constexpr array_view(pointer_type a, std::size_t items)
        : m_data(a), m_size(items) {
        }
        constexpr array_view(pointer_const_type a, std::size_t items)
        : m_data(a), m_size(items) {
        }
        array_view(pointer_type b, pointer_type e)
        : m_data(b), m_size(e - b) {
        }
        array_view(pointer_const_type b, pointer_const_type e)
        : m_data(b), m_size(e - b) {
        }

        /// Converting constructors
        template<typename W, typename E = std::enable_if_t<
                std::is_same<W, std::remove_const_t<V>>::value
            >>
        array_view(array_view<W> a)
        : m_data(a.data()), m_size(a.size()) {
        }

        /// The start of the data array
        constexpr std::add_pointer_t<V> data() {
            return m_data;
        }
        constexpr pointer_const_type data() const {
            return m_data;
        }
        /// The number of items in the array
        constexpr std::size_t size() const {
            return m_size;
        }

        /// Return a slice of this array
        constexpr array_view slice(std::size_t start) const {
            return array_view(m_data + start, m_size - start);
        }
        constexpr array_view slice(std::size_t start, std::size_t items) const {
            return array_view(m_data + start, items);
        }

        /// Index into the arraay
        constexpr V &operator [] (std::size_t index) {
            return data()[index];
        }
        constexpr const V &operator [] (std::size_t index) const {
            return data()[index];
        }

        /// Constant iterator
        using const_iterator = pointer_const_type;
        /// Start iterator
        const_iterator begin() const {
            return data();
        }
        /// End iterator
        const_iterator end() const {
            return data() + size();
        }

        /// Reverse const iterator
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        /// Start at the end
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }
        /// End at the beginning
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }
    };


}

