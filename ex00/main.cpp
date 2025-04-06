
#include <iostream>

#include "BitcoinExchange.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Error: invalid number of arguments" << std::endl;
        exit(1);
    }

    try {
        auto btc = BitcoinExchange();
        btc.calculateRates(argv[1]);
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "Error: unknown exception occured" << std::endl;
        exit(2);
    }
}
