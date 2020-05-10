/**
    Copyright 2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/cord/iostream.hpp>
#include <f5/cord/unicode.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <span>


namespace {
    using clock = std::chrono::steady_clock;

    template<typename V>
    struct datum {
        V v = {};
        clock::time_point t = {};

        void save(V nv) {
            v = std::move(nv);
            t = clock::now();
        }
    };
    template<typename S, typename V>
    struct stats {
        clock::time_point started = clock::now();
        datum<S> wordlist = {};
        datum<std::size_t> length = {};
        datum<std::pair<std::size_t, std::size_t>> words_letters = {};
        datum<std::vector<V>> words_view = {};
        datum<std::vector<S>> words = {};
    };
    template<class Ch, class Tr>
    inline auto &operator<<(std::basic_ostream<Ch, Tr> &os, clock::duration d) {
        auto const mscount =
                std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
        if (mscount) { return os << mscount << "ms"; }
        auto const μscount =
                std::chrono::duration_cast<std::chrono::microseconds>(d).count();
        if (μscount) { return os << μscount << "μs"; }
        return os << std::chrono::duration_cast<std::chrono::nanoseconds>(d)
                             .count()
                  << "ns";
    }
    template<class Ch, class Tr, typename S, typename V>
    inline auto &
            operator<<(std::basic_ostream<Ch, Tr> &os, stats<S, V> const &s) {
        auto duration = [&, from = s.started](clock::time_point until) mutable {
            auto r = until - from;
            from = until;
            return r;
        };
        os << "\n  construction " << duration(s.wordlist.t);
        os << "\n  length " << s.length.v << " " << duration(s.length.t);
        os << "\n  words " << s.words_letters.v.first << " letters "
           << s.words_letters.v.second << " " << duration(s.words_letters.t);
        os << "\n  word view arrray " << s.words_view.v.size() << " " << duration(s.words_view.t);
        os << "\n  words arrray " << s.words.v.size() << " " << duration(s.words.t);
        return os << '\n';
    }

    auto count(std::string const &s) { return s.size(); }
    auto count(f5::u8view s) { return s.memory().size(); }

    void emplace(std::vector<std::string_view> &w, std::string::const_iterator p, std::string::const_iterator e) {
        w.emplace_back(&*p, e - p);
    }
    void emplace(std::vector<f5::u8view> &w, f5::u8string::const_iterator p, f5::u8string::const_iterator e) {
        w.emplace_back(p, e);
    }

    template<typename S, typename V>
    auto test(std::vector<char> const &data) {
        stats<S, V> s;
        s.wordlist.save(S{data.data(), data.size()});
        s.length.save(count(s.wordlist.v));
        std::size_t letters{}, words{};
        for (auto ch : s.wordlist.v) {
            if (ch == '\n')
                ++words;
            else
                ++letters;
        }
        s.words_letters.save({words, letters});

        std::vector<V> words_view;
        words_view.reserve(s.words_letters.v.first);
        for (auto pos{s.wordlist.v.begin()}, end{s.wordlist.v.end()}; pos != end;) {
            auto ends = std::find(pos, end, '\n');
            emplace(words_view, pos, end);
            pos = ends;
            if (pos != end) ++pos;
        }
        s.words_view.save(std::move(words_view));
        s.words.save(std::vector<S>{s.words_view.v.begin(), s.words_view.v.end()});

        return s;
    }
    template<typename S, typename V>
    auto benchmark(std::vector<char> const &data) {
        /// Warm the cache otherwise it's too unfair....
        for (auto c{30}; c; --c) { test<S, V>(data); }
        return test<S, V>(data);
    }
}


int main(int const argc, char const *const argv[]) {
    try {
        std::span<char const *const> args{argv + 1, std::size_t(argc) - 1u};
        for (auto file : args) {
            std::cout << file;
            auto const bytes = std::filesystem::file_size(file);

            std::vector<char> wordlist(bytes);
            std::ifstream{file}.read(wordlist.data(), wordlist.size());
            std::cout << " " << wordlist.size() << " bytes\n";

            std::cout << "f5::u8string"
                      << benchmark<f5::u8string, f5::u8view>(wordlist);
            std::cout << "std::string"
                      << benchmark<std::string, std::string_view>(wordlist);
        }
    } catch (std::exception const &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
