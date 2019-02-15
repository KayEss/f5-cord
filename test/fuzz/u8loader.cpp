/**
    Copyright 2019, Felspar Co Ltd. <http://www.kirit.com/f5>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <f5/cord/unicode.hpp>

#include <fstream>
#include <iostream>


int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cout << "Display information about UTF-8 files\n"
                     "    "
                  << argv[0] << " u8filename" << std::endl;
        return 1;
    }
    /// Load the data from the file
    std::ifstream input{argv[1]};
    std::vector<char> bytes;
    while (input) {
        char c;
        input.get(c);
        bytes.push_back(c);
    }
    try {
        f5::u8string string{std::string{bytes.data(), bytes.size()}};
        /// Display meta-data about the file content
        std::cout << "Bytes: " << string.memory().size() << std::endl;
        std::cout << "Code points: " << string.code_points() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error caught: " << e.what() << std::endl;
        return 2;
    }
    return 0;
}
