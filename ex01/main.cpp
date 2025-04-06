#include <iostream>

#include "RPN.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        exit(1);
    }
    try {
        RPN why(argv[1]);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
