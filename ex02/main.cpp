#include <deque>
#include <iostream>
#include <vector>

#include "PmergeMe.hpp"
int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "No arguments given" << std::endl;
        exit(1);
    }
    try {
        PmergeMe<std::deque<int>> dequeSort(argv + 1);
        dequeSort.sort();
        dequeSort.validate();

        PmergeMe<std::vector<int>> vectorSort(argv + 1);
        vectorSort.sort();
        vectorSort.validate();
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
