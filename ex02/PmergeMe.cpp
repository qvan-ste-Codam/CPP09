#include "PmergeMe.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

PmergeMe::PmergeMe(const PmergeMe& other) : input(other.input) {}

PmergeMe& ::PmergeMe::operator=(const PmergeMe & other) {
    if (this != &other) {
        input = other.input;
    }
    return *this;
}

int safeStoi(const std::string& input) {
    try {
        size_t pos = 0;
        int i = std::stoi(input, &pos);

        if (pos != input.length()) {
            throw std::runtime_error("not a valid int => " + input);
        }
        return i;
    } catch (const std::exception& e) {
        throw std::runtime_error("not a valid int => " + input);
    }
}

PmergeMe::PmergeMe(char** argv) {
    while (*argv) {
        std::string item(*argv);
        input.push_back(safeStoi(item));
        argv++;
    }
    for (int i : input) {
        std::cout << i << std::endl;
    }
}
