/**
    Copyright 2018-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/cord/unicode-view.hpp>


namespace f5 {


    namespace cord {


        /// UTF8 string with shared ownership.
        template<typename C, typename V = basic_view<C>>
        class basic_string {
            using view_type = V;
            typename view_type::buffer_type buffer;
            using control_type = typename view_type::control_type;
            control_type *owner;

            /**
             * Temporary re-allocation function used to handle the cases
             * where the string might have been created from a `basic_view`
             * that was created from a `std::string`.
             *
             * This means that the `owner` will never be `nullptr` for any
             * `basic_string`.
             */
            void transitional_allocation() {
                if (owner == nullptr) { shrink_to_fit(); }
            }

          public:
            /// ## Types

            /// See [unicode-view.hpp](./unicode-view.hpp) for meanings
            /// of type names.
            using buffer_type = typename view_type::buffer_type;
            using value_type = typename view_type::value_type;
            using iterator_map = typename view_type::iterator_map;
            using std_string = typename view_type::std_string;
            using std_string_view = typename view_type::std_string_view;

            using size_type = typename view_type::size_type;
            constexpr static size_type const npos = view_type::npos;


            /// ## Constructors

            basic_string() noexcept : buffer{}, owner{} {}

            /// The type is copyable and movable. Handle the control block
            /// appropriately.
            basic_string(basic_string const &b)
            : buffer{b.buffer}, owner{control_type::increment(b.owner)} {}
            basic_string(basic_string &&b)
            : buffer{b.buffer}, owner{std::exchange(b.owner, nullptr)} {}

            /// Creation from a `basic_view` will never allocate because the
            /// `basic_view` remembers the shared status of its history.
            /// During the transitional period however basic_view *may*
            /// allocate.
            basic_string(view_type const v)
            : buffer{buffer_type{v}},
              owner{control_type::increment(v.control_block())} {
                transitional_allocation();
            }

            /// From literals we have a `nullptr` control block as we have
            /// nothing to count
            basic_string(lstring l) noexcept
            : buffer{l.data(), l.size()}, owner{} {}
            template<std::size_t N>
            basic_string(value_type (&a)[N]) noexcept
            : buffer{a, N - 1}, owner{} {}

            /// For `std_string` we have to move the string into a memory area
            /// we can control
            explicit basic_string(std_string s) : buffer{}, owner{} {
                auto created = control_type::make(std::move(s), s.size());
                owner = created.first.release();
                buffer = buffer_type{
                        created.second->data(), created.second->size()};
            }
            /// Given a data block we are going to have to allocate as well
            basic_string(value_type const *data, std::size_t size)
            : basic_string(std_string{data, size}) {}

            /// Construct from character literals in the non-native encodings
            template<typename O, std::size_t N>
            explicit basic_string(O const (&s)[N])
            : basic_string{[&]() {
                  using o_view =
                          basic_view<O, typename view_type::encoding_error_type>;
                  o_view nn{s};
                  std_string ret;
                  ret.reserve(N);
                  for (auto u32 : nn) {
                      auto const en = iterator_map::encode_one(u32);
                      ret.append(en.second.data(), en.first);
                  }
                  return ret;
              }()} {}

            ~basic_string() { control_type::decrement(owner); }


            /// ## Conversions
            operator view_type() const { return view_type{buffer, owner}; }
            operator std_string_view() const noexcept {
                return static_cast<std_string_view>(
                        static_cast<view_type>(*this));
            }

            explicit operator buffer_type() const { return buffer; }
            explicit operator f5::buffer<byte const>() const {
                return static_cast<f5::buffer<byte const>>(
                        static_cast<view_type>(*this));
            }
            explicit operator std_string() const {
                return static_cast<std_string>(static_cast<view_type>(*this));
            }

            /// Force re-allocation of the memory such that we also have
            /// a NUL at the end of string. This guarantees that the string
            /// is now also C safe. We make this operation idempotent by
            /// storing the number of bytes used in the original string
            /// allocation so that we can check if we need to re-allocate s
            /// new string or not.
            value_type const *shrink_to_fit() {
                if ((owner && owner->user_data != code_units()) || not owner) {
                    *this = basic_string{static_cast<std_string>(*this)};
                }
                return data();
            }

            /// ## Assignment
            basic_string &operator=(const basic_string &s) noexcept {
                control_type::decrement(owner);
                buffer = s.buffer;
                owner = control_type::increment(s.owner);
                return *this;
            }
            basic_string &operator=(basic_string &&s) noexcept {
                control_type::decrement(owner);
                buffer = s.buffer;
                owner = std::exchange(s.owner, nullptr);
                return *this;
            }

            /// ## Iteration

            /// An iterator that produces UTF32 code points
            using const_iterator = typename view_type::const_iterator;
            const_iterator begin() const noexcept {
                return static_cast<view_type>(*this).begin();
            }
            const_iterator end() const noexcept {
                return static_cast<view_type>(*this).end();
            }

            /// Construct from a pair of iterators
            basic_string(const_iterator b, const_iterator e) noexcept
            : buffer{iterator_map::template get_buffer<buffer_type, control_type>(
                    b, e)},
              owner{control_type::increment(b.owner)} {
                assert(b.owner == e.owner);
                transitional_allocation();
            }

            /// An iterator that produces UTF16 code units from the string
            using const_u16_iterator = typename view_type::const_u16_iterator;

            /// Return the begin iterator that delivers UTF16 code units
            const_u16_iterator u16begin() const {
                return const_u16_iterator(begin(), end());
            }
            /// Return the end iterator that delivers UTF16 code units
            const_u16_iterator u16end() const {
                return const_u16_iterator(end(), end());
            }


            /// ## Queries

            /// Returns `true` if this is a shared string
            bool is_shared() const noexcept { return owner != nullptr; }
            /// Returns true if the other string uses the same allocation
            /// as this (they have the same control block).
            bool shares_allocation_with(view_type const v) noexcept {
                return owner != nullptr && owner == v.control_block();
            }
            /// Return the memory control block
            control_type *control_block() const noexcept { return owner; }

            /// Return the data array
            value_type *data() const noexcept { return buffer.data(); }
            /// Return the size in bytes of the string
            std::size_t bytes() const noexcept {
                return buffer.size() * sizeof(value_type);
            }
            /// Return the number of code units
            std::size_t code_units() const noexcept { return buffer.size(); }
            /// Return the size in code points
            auto code_points() const { return std::distance(begin(), end()); }
            /// Return true if the string is empty
            bool empty() const noexcept { return buffer.empty(); }
            /// Return the underlying memory block for the data
            auto memory() const noexcept { return view_type{*this}.memory(); }

            /// Useful checks for parts of a string
            bool starts_with(view_type str) const {
                return view_type{buffer.slice(0, str.code_units())} == str;
            }
            bool ends_with(view_type str) const {
                return view_type{*this}.ends_with(str);
            }


            /// ## Substrings

            /// Safe substring against Unicode code point counts. The result
            /// is undefined if the end marker is smaller than the start marker.
            basic_string substr(std::size_t s) const {
                auto pos = begin();
                auto const e = end();
                for (; s && pos != e; --s, ++pos)
                    ;
                return basic_string(pos, e);
            }
            basic_string substr_pos(std::size_t s, std::size_t e) const {
                auto starts = substr(s);
                auto ends = starts.substr(e - s);
                return basic_string(starts.begin(), ends.begin());
            }


            /// ## Comparisons

            /// Comparison. Acts as a string would. Not unicode aware in
            /// that it doesn't take into account normalisation, it only
            /// compares the byte values.
            friend bool
                    operator==(basic_string const &l, basic_string const &r) {
                return view_type{l} == view_type{r};
            }
            template<typename O>
            friend std::enable_if_t<is_detected_v<op_meq_t, view_type, O>, bool>
                    operator==(basic_string const &s, O const &l) {
                return view_type{s} == l;
            }
            template<typename O>
            friend std::enable_if_t<is_detected_v<op_meq_t, view_type, O>, bool>
                    operator==(O const &r, basic_string const &s) {
                return view_type{s} == r;
            }

            friend bool
                    operator!=(basic_string const &l, basic_string const &r) {
                return view_type{l} != view_type{r};
            }
            template<typename O>
            friend std::enable_if_t<is_detected_v<op_meq_t, view_type, O>, bool>
                    operator!=(basic_string const &s, O const &l) {
                return view_type{s} != l;
            }
            template<typename O>
            friend std::enable_if_t<is_detected_v<op_meq_t, view_type, O>, bool>
                    operator!=(O const &r, basic_string const &s) {
                return view_type{s} != r;
            }

            bool operator<(view_type l) const { return view_type{buffer} < l; }
            bool operator<=(view_type l) const {
                return view_type{buffer} <= l;
            }
            bool operator>=(view_type l) const {
                return view_type{buffer} >= l;
            }
            bool operator>(view_type l) const { return view_type{buffer} > l; }
        };


        /// String types
        using u8string = basic_string<char>;
        using u16string = basic_string<char16_t>;
        using u32string = basic_string<char32_t>;


        /// ## Concatenation
        template<typename C>
        inline basic_string<C> operator+(basic_view<C> f, basic_view<C> e) {
            std::basic_string<C> r;
            r.reserve(f.code_units() + e.code_units());
            r.append(f.data(), f.code_units());
            r.append(e.data(), e.code_units());
            return basic_string<C>{r};
        }
        template<typename C>
        inline auto operator+(basic_string<C> l, basic_view<C> r) {
            return operator+(basic_view<C>{l}, r);
        }
        template<typename C>
        inline auto operator+(basic_string<C> l, basic_string<C> r) {
            return operator+(basic_view<C>{l}, basic_view<C>{r});
        }
        template<std::size_t N>
        inline u8string operator+(u8view f, char const (&e)[N]) {
            return operator+(f, u8view{e});
        }
        template<std::size_t N>
        inline u8string operator+(const u8string &f, char const (&e)[N]) {
            return operator+(u8view{f}, u8view{e});
        }
        inline u8string operator+(u8view f, const std::string &e) {
            return operator+(f, u8view{e.data(), e.size()});
        }
        inline u8string operator+(u8string f, const std::string &e) {
            return operator+(u8view{f}, u8view{e.data(), e.size()});
        }
        template<typename C>
        inline basic_string<C>
                operator+(std::basic_string<C> const &f, basic_view<C> const e) {
            return operator+(basic_view<C>{f.data(), f.size()}, e);
        }
        template<typename C>
        inline basic_string<C> operator+(
                std::basic_string<C> const &f, basic_string<C> const e) {
            return operator+(
                    basic_view<C>{f.data(), f.size()}, basic_view<C>{e});
        }
        template<std::size_t N>
        inline u8string operator+(char const (&f)[N], u8view e) {
            return operator+(u8view{f}, e);
        }


    }


    /// Aliases directly in `f5` as these will be used quite often
    using u8string = cord::u8string;
    using u16string = cord::u16string;
    using u32string = cord::u32string;


}
