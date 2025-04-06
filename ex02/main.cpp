#include <iostream>

#include "PmergeMe.hpp"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "No arguments given";
        exit(1);
    }
    try {
        PmergeMe compliSort(argv + 1);
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
}
