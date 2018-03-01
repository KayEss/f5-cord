/*
    Copyright 2017, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <array>
#include <memory>
#include <type_traits>
#include <vector>


namespace f5 {


    /// Basic view of contiguous blocks of some type. Two views
    /// are the same only if they point to the same underlying memory.
    template<typename V>
    class buffer final {
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
        buffer()
        : m_data(nullptr), m_size(0u) {
        }

        /// Construct from a vector
        buffer(std::vector<value_type> &v)
        : m_data(v.data()), m_size(v.size()) {
        }
        template<typename T>
        buffer(const std::vector<T> &v)
        : m_data(v.data()), m_size(v.size()) {
        }
        // For C++ arrays
        template<typename T, std::size_t N>
        constexpr buffer(std::array<T, N> &v)
        : m_data(v.data()), m_size(N) {
        }
        template<typename T, std::size_t N>
        constexpr buffer(const std::array<T, N> &v)
        : m_data(v.data()), m_size(N) {
        }
        /// From a C array
        template<std::size_t N>
        constexpr buffer(V (&a)[N])
        : m_data(a), m_size(N) {
        }

        /// Construct from pointers
        constexpr buffer(pointer_type a, std::size_t items)
        : m_data(a), m_size(items) {
        }
        constexpr buffer(pointer_const_type a, std::size_t items)
        : m_data(a), m_size(items) {
        }
        buffer(pointer_type b, pointer_type e)
        : m_data(b), m_size(e - b) {
        }
        buffer(pointer_const_type b, pointer_const_type e)
        : m_data(b), m_size(e - b) {
        }

        /// Converting constructors
        template<typename W, typename E = std::enable_if_t<
                std::is_same<W, std::remove_const_t<V>>::value
            >>
        buffer(buffer<W> a)
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
        /// Return true if there are no items in the array
        constexpr bool empty() const {
            return m_size == 0;
        }

        /// Return a slice of this array
        constexpr buffer slice(std::size_t start) const {
            return buffer(m_data + start, m_size - start);
        }
        constexpr buffer slice(std::size_t start, std::size_t items) const {
            return buffer(m_data + start, items);
        }

        /// Ordering. Performs element-wise ordering. In a tie the shortest
        /// is less than the longest.
        constexpr bool operator < (buffer r) const {
            const auto checks = m_size < r.m_size ? m_size : r.m_size;
            for ( std::size_t s{}; s != checks; ++s ) {
                if ( m_data[s] != r.m_data[s] ) return m_data[s] < r.m_data[s];
            }
            return m_size < r.m_size;
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


    /// An buffer whose ownership is shared
    template<typename V>
    class shared_buffer {
        std::shared_ptr<V> m_data;
        std::size_t m_size;

        shared_buffer(std::shared_ptr<V> ptr, std::size_t begin, std::size_t count)
        : m_data(std::move(ptr), ptr.get() + begin), m_size(count) {
        }
    public:
        /// Buffer types
        using buffer_type = buffer<std::remove_const_t<V>>;
        using const_buffer_type = buffer<std::add_const_t<V>>;

        /// Pointer types
        using pointer_type = typename buffer_type::pointer_type;
        using pointer_const_type = typename buffer_type::pointer_const_type;

        /// Construct buffer large enough to hold `size` items, which are
        /// all default constructed.
        shared_buffer(std::size_t size)
        : m_data(new V[size], [](auto p) { delete[] p; }), m_size(size) {
        }

        /// The number of elements in the buffer
        std::size_t size() const {
            return m_size;
        }

        /// Access to the underlying memory block
        pointer_type data() {
            return m_data.get();
        }
        pointer_const_type data() const {
            return m_data.get();
        }

        /// Subscription operators into the memory
        V& operator [] (std::size_t index) {
            return data()[index];
        }

        /// Return a slice which is also shared
        shared_buffer slice(std::size_t index) {
            return shared_buffer(m_data, index, m_size - index);
        }
        shared_buffer slice(std::size_t index, std::size_t count) {
            return shared_buffer(m_data, index, count);
        }

        /// Iteration across the memory
        using iterator = V*;
        iterator begin() {
            return data();
        }
        iterator end() {
            return data() + m_size;
        }

        /// Conversion to non-owning buffers
        operator buffer<V> () {
            return buffer<V>{m_data.get(), m_size};
        }
        operator buffer<const V> () const {
            return buffer<const V>{m_data.get(), m_size};
        }
    };


}

